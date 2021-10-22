#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>

#include "SimpleVUEffect.h"
#include "CenteredVUEffect.h"
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
#define VOLUME_ADJUST_CHARACTERISTIC_UUID "da5d9c27-d269-4ff9-82b6-f2931c92f73d"
#define VOLUME_MODIFIER_CHARACTERISTIC_UUID "cc67937a-1d5d-48ce-8efd-409bb64a6d8a"

#define MODE_OFF_STR "off"
#define MODE_CYCLE_STR "cycle"
#define MODE_SELECT_STR "select"

#define MODE_OFF 0
#define MODE_CYCLE 1
#define MODE_SELECT 2

#define VOLUME_MODE_AUTO "auto"
#define VOLUME_MODE_SELECT "select"

const unsigned char MAX_PATTERNS = 2;
const unsigned int CYCLE_RATE = 20000;

unsigned char currentPattern = 0;
unsigned char currentMode = MODE_OFF;
unsigned long lastActivityTimestamp = millis();
bool adjustVolumeModifierAutomatically = false;
unsigned char volumeModifierChar = 255;
float volumeModifier = 1.0f;

unsigned int sample;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

LightEffect* currentEffect = 0;

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

void switchEffect(unsigned char effect) {
	Serial.print(F("Switching to effect: "));Serial.println(effect);
	switch(effect) {
		case 0:
			switchEffect(new SimpleVUEffect());
			break;
		case 1:
			switchEffect(new CenteredVUEffect());
			break;
		default:
			switchEffect(new SimpleVUEffect());
			break;
	}
}

class CharacteristicChangeCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string rxValue = pCharacteristic->getValue();
        if (rxValue.length() > 0) {
            Serial.print(pCharacteristic->getUUID().toString().c_str());
            Serial.print(F(" Received Value: "));
            if(pCharacteristic->getUUID().toString().compare(MODE_CHARACTERISTIC_UUID) == 0) {
                Serial.print(F("(Mode change) "));Serial.println(rxValue.c_str());

                // handle mode changes
                if(rxValue.compare(MODE_OFF_STR) == 0) {
                    currentMode = MODE_OFF;
                } else if(rxValue.compare(MODE_CYCLE_STR) == 0) {
                    currentMode = MODE_CYCLE;
                    lastActivityTimestamp = millis();
                } else if(rxValue.compare(MODE_SELECT_STR) == 0) {
                    currentMode = MODE_SELECT;
                }
                Serial.print(F("Mode changed to: "));Serial.print(currentMode);
            } else if (pCharacteristic->getUUID().toString().compare(PATTERN_CHARACTERISTIC_UUID) == 0) {
                Serial.print(F("(Pattern change) "));Serial.println((unsigned char)rxValue[0]);

                // handle pattern changes
                if((unsigned char)rxValue[0] < MAX_PATTERNS) {
                    currentPattern = (unsigned char)rxValue[0];
                    switchEffect(currentPattern);
                }
            } else if (pCharacteristic->getUUID().toString().compare(VOLUME_ADJUST_CHARACTERISTIC_UUID) == 0) {
            	Serial.print(F("(Volume mode change) "));Serial.print(rxValue.c_str());

            	// handle mode changes
				if(rxValue.compare(VOLUME_MODE_AUTO) == 0) {
					adjustVolumeModifierAutomatically = true;
				} else if(rxValue.compare(VOLUME_MODE_SELECT) == 0) {
					adjustVolumeModifierAutomatically = false;
				}
            } else if (pCharacteristic->getUUID().toString().compare(VOLUME_MODIFIER_CHARACTERISTIC_UUID) == 0) {
            	Serial.print(F("(Volume modifier) "));Serial.print((unsigned char)rxValue[0]);

            	volumeModifierChar = rxValue[0];
            	volumeModifier = (float)volumeModifierChar / 255;
            }
            Serial.println();
        }
    }
};

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

    BLECharacteristic *pVolumeModeCharacteristic = pService->createCharacteristic(
		VOLUME_ADJUST_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE|BLECharacteristic::PROPERTY_NOTIFY
	);
	pVolumeModeCharacteristic->setValue(VOLUME_MODE_SELECT);
	pVolumeModeCharacteristic->setCallbacks(callbacks);

	BLECharacteristic* pVolumeModifierCharacteristic = pService->createCharacteristic(
            VOLUME_MODIFIER_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE|BLECharacteristic::PROPERTY_NOTIFY
        );
	pVolumeModifierCharacteristic->setValue(&volumeModifierChar, 1);
	pVolumeModifierCharacteristic->setCallbacks(callbacks);

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    strip.begin();
    strip.show();

	pinMode(MIC_PIN, INPUT);

	switchEffect(new SimpleVUEffect());

    Serial.println(F("Lightsuit started"));
}

void loop() {
	if(currentMode == MODE_OFF) {
		return;
	}

	// cycle patterns if needed
	if(currentMode == MODE_CYCLE && lastActivityTimestamp + CYCLE_RATE < millis()) {
		lastActivityTimestamp = millis();
		currentPattern++;

		if(currentPattern >= MAX_PATTERNS) {
			currentPattern = 0;
		}
		switchEffect(currentPattern);
	}

	double currentSample = getVolumeSample();

	currentSample = currentSample * 1; // TODO: replace 5 with multiplier value set via BLE

	if(currentSample > 1) {
		currentSample = 1.0;
	}

	VolumeContext context;
	context.volume = currentSample;
	context.ledStrip.strip = &strip;
	context.ledStrip.numLeds = NUM_LEDS;

	if(currentEffect != 0) {
		currentEffect->loop(context);
		strip.show();
	}
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
