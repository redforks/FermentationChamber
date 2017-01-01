#include "fermch.h"
#include "read_temp.h"
#include <core.h>
#include <dht.h>

using namespace core;

dht DHT;

// Return false if read failed.
bool doReadDHT22() {
  int chk = DHT.read22(DHT22_PIN);

  switch (chk) {
    case DHTLIB_OK:
      return true;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print(F("Checksum error,\n"));
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print(F("Time out error,\n"));
      break;
    default:
      Serial.print(F("Unknown error,\n"));
      break;
  }

  return false;
}

idType idTempe;

void setTempe(int16_t tempe) {
  Serial.print(F("temp: "));
  Serial.println(tempe);
  store::setAnalog(idTempe, (uint16_t)tempe);
}

void updateTemperature(int16_t temp) {
  setTempe(temp);
}

void readDHT22() {
  if (!doReadDHT22()) {
    return;
  }

  updateTemperature(DHT.temperature);
}

void readTempeFirstTime() {
  int16_t tempe = SETPOINT;
  if (doReadDHT22()) {
    tempe = DHT.temperature;
  }

  updateTemperature(tempe);

  clock::interval(DHT22_SAMPLE_RATE, readDHT22);
}

void setupTempe() {
  idTempe = store::defineAnalog();

  setTempe(200);

  // At this time, no other modules hooks idTempe & idHumi,
  // delays inital read, to trigger interested modules.
  // DHT22 datasheet said, should not read DHT22 in first second after power up.
  clock::delay(1500, readTempeFirstTime);
}
