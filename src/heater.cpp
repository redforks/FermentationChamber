#include <core.h>
#include "heater.h"

using namespace core;

idType idHeater;

// current(last) heater actual on/off state. Roaster oven has too much power,
// use pulse logic, a power cycle is 55 seconds, 5 seconds on, 50 seconds off,
// to reduce heater power to 1/10.
bool mLastPulseOn;

uint32_t mLastActionTime; // milli seconds of last on/off since arduino boot.

void* heaterPulseOnDelayHandler = NULL;

void heaterPulseOn() {
  uint32_t delay = millis() - mLastActionTime;
  if (mLastPulseOn) {
    if (delay >= 5000) {
      mLastPulseOn = false;
      digitalWrite(HEATER_PIN, LOW);
      mLastActionTime = millis();
    }
  } else if (delay >= 200000 || millis() < 10000) {
    mLastPulseOn = true;
    digitalWrite(HEATER_PIN, HIGH);
    mLastActionTime = millis();
  }
}

void switchHeater() {
  if (store::digitals[idHeater] == LOW) {
    clock::removeInterval(heaterPulseOnDelayHandler);
    heaterPulseOnDelayHandler = NULL;

    digitalWrite(HEATER_PIN, LOW);
    return;
  }

  if (heaterPulseOnDelayHandler == NULL) {
    heaterPulseOnDelayHandler = clock::interval(5000, &heaterPulseOn);
    heaterPulseOn();
  }
}

void setupHeater(void) {
  pinMode(HEATER_PIN, OUTPUT);

  idHeater = store::defineDigital();
  store::monitorDigitals(switchHeater, 1, idHeater);
}
