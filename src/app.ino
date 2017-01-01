#include <core.h>
#include "fermch.h"
#include "heater.h"
#include "read_temp.h"
#include "tempe_control.h"
#include "display.h"

using namespace core;
using namespace core::store;

void setup() {
  Serial.begin(115200);
  Serial.println(F("## Fermentation Chamber by Red Forks ##\n"));

  setupDisplay();
  setupTempe();
  setupHeater();
  setupTempeControl();
}

void loop() {
  clock::check();
}
