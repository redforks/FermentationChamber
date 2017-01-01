#include <LiquidCrystal.h>
#include <core.h>
#include "fermch.h"
#include "display.h"
#include "read_temp.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

using namespace core;

void print2Digits(uint16_t number, char prefix) {
  if (number < 10) {
    lcd.print(prefix);
  }
  lcd.print(number);
}

void print2DigitsSpace(uint16_t number) {
  print2Digits(number, ' ');
}

// print number like 12.5, store as 125.
void printNumber00n0(uint16_t number) {
  print2DigitsSpace(number / 10);
  lcd.print('.');
  lcd.print(number % 10);
}

void onTempeChanges() {
  lcd.setCursor(1, 0);
  int16_t tempe = readTempe();
  if (tempe < 0) {
    lcd.setCursor(0, 0);
    lcd.print('-');
    tempe = -tempe;
  } else {
    lcd.print(' '); // clear the possible negative '-'
  }
  printNumber00n0(tempe);
}

void onHeaterChanges() {
  lcd.setCursor(9, 0);

  uint8_t state = store::digitals[idHeater];
  if (state == HIGH) {
    lcd.print(F("On "));
  } else {
    lcd.print(F("Off"));
  }
}

void setupDisplay() {
  lcd.begin(16, 2);

  lcd.clear();
  lcd.print(F("Fermentation"));
  lcd.setCursor(0, 1);
  lcd.print(F("Chamber v1.0"));

  delay(2000);
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print(F("\337C"));

  store::monitorAnalogs(onTempeChanges, 1, idTempe);
  store::monitorDigitals(onHeaterChanges, 1, idHeater);
}
