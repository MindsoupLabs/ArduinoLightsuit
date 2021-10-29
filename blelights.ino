#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>

#include "SimpleVUEffect.h"
#include "CenteredVUEffect.h"
#include "JitteryMultiMiniVUEffect.h"
#include "RunningLightEffect.h"
#include "ShootingLightEffect.h"
#include "SparkleLightEffect.h"
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
#define DEMO_CHARACTERISTIC_UUID "cc67937a-1d5d-48ce-8efd-409bb64a6d8a"

#define MODE_OFF_STR "off"
#define MODE_CYCLE_STR "cycle"
#define MODE_SELECT_STR "select"
#define MODE_RANDOM_STR "random"

#define MODE_OFF 0
#define MODE_CYCLE 1
#define MODE_SELECT 2
#define MODE_RANDOM 3

#define VOLUME_MODE_AUTO "auto"
#define VOLUME_MODE_SELECT "select"

const unsigned char MAX_PATTERNS = 6;
const unsigned int CYCLE_RATE = 15000;

// start values
unsigned char currentPattern = 0;
unsigned char currentMode = MODE_OFF;

// test values
//unsigned char currentPattern = 1;
//unsigned char currentMode = MODE_SELECT;

unsigned long lastActivityTimestamp = millis();
bool adjustVolumeModifierAutomatically = true;

bool isInDemoMode = false;

unsigned int sample;
const unsigned int shortSamplesAmount = ceil(1000 / SAMPLE_TIME_MS);
double shortSamplesBuffer[shortSamplesAmount];
unsigned char shortSamplesIndex = 0;
const unsigned char longSamplesAmount = 10;
double longSamplesBuffer[longSamplesAmount];
unsigned char longSamplesIndex = 0;

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
			switchEffect(new RunningLightEffect());
			break;
		case 2:
			switchEffect(new ShootingLightEffect());
			break;
		case 3:
			switchEffect(new CenteredVUEffect());
			break;
		case 4:
			switchEffect(new JitteryMultiMiniVUEffect());
			break;
		case 5:
			switchEffect(new SparkleLightEffect());
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
                    strip.fill(strip.Color(0,0,0), 0, NUM_LEDS);
                    strip.show();
                } else if(rxValue.compare(MODE_CYCLE_STR) == 0) {
                    currentMode = MODE_CYCLE;
                    lastActivityTimestamp = millis();
                } else if(rxValue.compare(MODE_SELECT_STR) == 0) {
                    currentMode = MODE_SELECT;
                } else if(rxValue.compare(MODE_RANDOM_STR) == 0) {
					currentMode = MODE_RANDOM;
				}
                Serial.print(F("Mode changed to: "));Serial.print(currentMode);
            } else if (pCharacteristic->getUUID().toString().compare(PATTERN_CHARACTERISTIC_UUID) == 0) {
                Serial.print(F("(Pattern change) "));Serial.println((unsigned char)rxValue[0]);
				Serial.println((unsigned char)rxValue[0] < MAX_PATTERNS);
                // handle pattern changes
                if((unsigned char)rxValue[0] < MAX_PATTERNS) {
                    currentPattern = (unsigned char)rxValue[0];
                    Serial.println(currentPattern);
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
            } else if (pCharacteristic->getUUID().toString().compare(DEMO_CHARACTERISTIC_UUID) == 0) {
            	Serial.print(F("(Demo setting) "));Serial.print(rxValue.c_str());

            	if(rxValue.compare(MODE_OFF_STR) == 0) {
					isInDemoMode = false;
            	} else {
            		isInDemoMode = true;
            	}
            	Serial.print(", demo is active: ");Serial.print(isInDemoMode);
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

	BLECharacteristic* pDemoCharacteristic = pService->createCharacteristic(
            DEMO_CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE|BLECharacteristic::PROPERTY_NOTIFY
        );
	pDemoCharacteristic->setValue(MODE_OFF_STR);
	pDemoCharacteristic->setCallbacks(callbacks);

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

	switchEffect(currentPattern);

    Serial.println(F("Lightsuit started"));
}

void loop() {
	if(currentMode == MODE_OFF) {
		return;
	}

	// cycle patterns if needed
	if( (currentMode == MODE_CYCLE || currentMode == MODE_RANDOM) && lastActivityTimestamp + CYCLE_RATE < millis()) {
		lastActivityTimestamp = millis();

		if(currentMode == MODE_CYCLE) {
			currentPattern++;
		} else {
			currentPattern = floor((1.0 * esp_random() / UINT32_MAX) * MAX_PATTERNS);
		}

		if(currentPattern >= MAX_PATTERNS) {
			currentPattern = 0;
		}
		switchEffect(currentPattern);
	}

	double currentSample = isInDemoMode ? getDemoVolumeSample() : getVolumeSample();

	// handle automatic volume adjustments
	shortSamplesBuffer[shortSamplesIndex] = currentSample;
	shortSamplesIndex++;

	// we have a second worth of samples, find the greatest value and reset the buffer index
	if(shortSamplesIndex >= shortSamplesAmount) {
		shortSamplesIndex = 0;

		double loudestVolume = 0.0;
		for(unsigned char i = 0; i < shortSamplesAmount; i++) {
			if(shortSamplesBuffer[i] > loudestVolume) {
				loudestVolume = shortSamplesBuffer[i];
			}
		}

		longSamplesBuffer[longSamplesIndex] = loudestVolume;

		// reset back to 0 after reaching longSamplesAmount - 1
		longSamplesIndex = longSamplesIndex >= longSamplesAmount - 1 ? 0 : longSamplesIndex + 1;
	}

	float multiplier = 1.0;
	if(adjustVolumeModifierAutomatically) {
		// find loudest sample of the last X seconds
		double loudestVolume = 0.0;
		float historicScaleFactor = 1.0; // older samples are weighed less
		for(unsigned char i = 0; i < longSamplesAmount; i++) {
			historicScaleFactor = (1.0 + i) / longSamplesAmount;
			if(longSamplesBuffer[(i + longSamplesIndex) % longSamplesAmount] * historicScaleFactor > loudestVolume) {
				loudestVolume = longSamplesBuffer[i];
			}
		}

		multiplier = 1.0 / loudestVolume * 0.9;
	}

	if(currentSample * multiplier < 1.0) {
        currentSample = currentSample * multiplier;
    }

    if(currentSample > 1.0) {
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

	// provide the square root to emphasise lower volumes and deemphasise higher volumes
	// this turns the linear 0.0 to 1.0 range into a curved 0.0 to 1.0 range
	return max(0.0, sqrt(normalized) - 0.2) / 0.8;

	//alternate option: sqrt(max(0.0, normalized - 0.04) / 0.96);
}

double getDemoVolumeSample() {
	delay(SAMPLE_TIME_MS);

	double randomValue = (1.0 * esp_random()) / UINT32_MAX;

	if(millis() % 500 > 440) {
		return 0.75 + (0.25 * randomValue);
	}

	return max(0.3 * randomValue, (randomValue * randomValue) - 0.2);
}
