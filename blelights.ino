#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "23914549-10c1-490d-a01b-9a4bea11a878"
#define MODE_CHARACTERISTIC_UUID "3cc3402b-c79c-4f9d-9369-2901a31a0692"
#define PATTERN_CHARACTERISTIC_UUID "9dec7a9f-effe-407d-ac8e-227e2dc64982"

#define MODE_OFF_STR "off"
#define MODE_CYCLE_STR "cycle"
#define MODE_SELECT_STR "select"

#define MODE_OFF 0
#define MODE_CYCLE 1
#define MODE_SELECT 2

#define ONBOARD_LED_PIN 2

const unsigned char MAX_PATTERNS = 4;
const unsigned int CYCLE_RATE = 4000;

unsigned char currentPattern = 0;
unsigned char currentMode = MODE_OFF;
unsigned long lastActivityTimestamp = millis();
bool ledIsOn = false;

BLECharacteristic* pPatternCharacteristic;

void notifyPatternChanged() {
    pPatternCharacteristic->setValue(&currentPattern, 1);
    pPatternCharacteristic->notify();
}

class CharacteristicChangeCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            Serial.print(pCharacteristic->getUUID().toString().c_str());
            Serial.print(" Received Value: ");
            if(pCharacteristic->getUUID().toString().compare(MODE_CHARACTERISTIC_UUID) == 0) {
                Serial.print(rxValue.c_str());

                // handle mode changes
                if(rxValue.compare(MODE_OFF_STR) == 0) {
                    currentMode = MODE_OFF;
                } else if(rxValue.compare(MODE_CYCLE_STR) == 0) {
                    currentMode = MODE_CYCLE;
                } else if(rxValue.compare(MODE_SELECT_STR) == 0) {
                    currentMode = MODE_SELECT;
                }
            } else if (pCharacteristic->getUUID().toString().compare(PATTERN_CHARACTERISTIC_UUID) == 0) {
                Serial.print((unsigned char)rxValue[0]);

                // handle pattern changes
                if((unsigned char)rxValue[0] < MAX_PATTERNS) {
                    currentPattern = (unsigned char)rxValue[0];
                    notifyPatternChanged();
                }
            }
            Serial.println();
        }
    }
};

void incrementCurrentPattern() {
    currentPattern++;

    if(currentPattern >= MAX_PATTERNS) {
        currentPattern = 0;
    }

    notifyPatternChanged();
}

void setup() {
    Serial.begin(115200);
    while(!Serial) {
        // wait
    }

    Serial.println(F("Starting lightsuit..."));

    // Create the BLE Device
    BLEDevice::init("Lightsuit");

    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    CharacteristicChangeCallbacks* callbacks = new CharacteristicChangeCallbacks();

    BLECharacteristic *pModeCharacteristic = pService->createCharacteristic(
        MODE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE|BLECharacteristic::PROPERTY_NOTIFY
    );
    pModeCharacteristic->setValue(MODE_OFF_STR);
    pModeCharacteristic->setCallbacks(callbacks);

    pPatternCharacteristic = pService->createCharacteristic(
        PATTERN_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE|BLECharacteristic::PROPERTY_NOTIFY
    );
    pPatternCharacteristic->setValue(&currentPattern, 1);
    pPatternCharacteristic->setCallbacks(callbacks);

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    pinMode(ONBOARD_LED_PIN, OUTPUT);

    Serial.println(F("Lightsuit started"));
}

void loop() {
    if(millis() - lastActivityTimestamp > CYCLE_RATE) {
        lastActivityTimestamp = millis();

        ledIsOn = !ledIsOn;
        digitalWrite(ONBOARD_LED_PIN, ledIsOn ? HIGH : LOW);

        if(currentMode == MODE_CYCLE) {
            incrementCurrentPattern();
        }

        Serial.print(F("Mode: "));Serial.print(currentMode);Serial.print(F(", Pattern: "));Serial.println(currentPattern);
    }
}
