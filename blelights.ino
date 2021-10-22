#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>

#include "SimpleVUEffect.h"
#include "VolumeContext.h"
#include "LedStripContext.h"

#define SERIAL_SPEED 115200

// microphone defines
#define MIC_PIN A0
#define SAMPLES_SIZE 128
#define MAX_MIC_VALUE 4096
#define SAMPLE_TIME_MS 50

// LED defines
#define NUM_LEDS 87
#define LED_PIN 13

// BLE defines
#define SERVICE_UUID "23914549-10c1-490d-a01b-9a4bea11a878"
#define MODE_CHARACTERISTIC_UUID "3cc3402b-c79c-4f9d-9369-2901a31a0692"
#define PATTERN_CHARACTERISTIC_UUID "9dec7a9f-effe-407d-ac8e-227e2dc64982"

#define MODE_OFF_STR "off"
#define MODE_CYCLE_STR "cycle"
#define MODE_SELECT_STR "select"

#define MODE_OFF 0
#define MODE_CYCLE 1
#define MODE_SELECT 2

const unsigned char MAX_PATTERNS = 4;
const unsigned int CYCLE_RATE = 4000;

unsigned char currentPattern = 0;
unsigned char currentMode = MODE_OFF;
unsigned long lastActivityTimestamp = millis();

unsigned int sample;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

LightEffect* currentEffect = 0;

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
}

void setup() {
    Serial.begin(SERIAL_SPEED);
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

    BLECharacteristic* pPatternCharacteristic = pService->createCharacteristic(
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

    strip.begin();
    strip.show();

	// put your setup code here, to run once:
	pinMode(MIC_PIN, INPUT);

	switchEffect(new SimpleVUEffect());

    Serial.println(F("Lightsuit started"));
}

void loop() {
	double currentSample = getVolumeSample();

	Serial.println(currentSample);

	currentSample = currentSample * 1; // TODO: replace 5 with multiplier value set via BLE

	if(currentSample > 1) {
		currentSample = 1.0;
	}

	VolumeContext context;
	context.volume = currentSample;
	context.ledStrip.strip = &strip;
	context.ledStrip.numLeds = NUM_LEDS;

	currentEffect->loop(context);
}

void switchEffect(LightEffect* effect) {
	if(currentEffect != 0) {
		delete currentEffect;
	}

	currentEffect = effect;

	LedStripContext context;
	context.strip = &strip;
	context.numLeds = NUM_LEDS;

	currentEffect->setup(context);
}

double getVolumeSample() {
	unsigned long startMillis = millis();  // Start of sample window
	float peakToPeak = 0;   // peak-to-peak level

	unsigned int signalMax = 0;
	unsigned int signalMin = MAX_MIC_VALUE;

	// collect data for 50 mS
	while (millis() - startMillis < SAMPLE_TIME_MS) {
		sample = analogRead(MIC_PIN);
		if (sample < MAX_MIC_VALUE) {
			if (sample > signalMax) {
				signalMax = sample;  // save just the max levels
			} else if (sample < signalMin) {
				signalMin = sample;  // save just the min levels
			}
		}
	}

	peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
	double normalized = peakToPeak / MAX_MIC_VALUE;

	return normalized;
}
