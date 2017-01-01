#include <core.h>
#include "fermch.h"
#include "read_temp.h"
#include "tempe_control.h"

using namespace core;

int16_t getTempeHysteresis() {
  return HYSTERESIS;
}

int16_t getTempeSetpoint() {
  return SETPOINT;
}

void temperatureLoop() {
  int16_t tempe = readTempe();
  if (tempe >= getTempeSetpoint()) {
    store::setDigital(idHeater, LOW);
  } else if (tempe <= (getTempeSetpoint() - getTempeHysteresis())) {
    store::setDigital(idHeater, HIGH);
  }
}

void setupTempeControl(void) {
  store::monitorAnalogs(temperatureLoop, 1, idTempe);
}
