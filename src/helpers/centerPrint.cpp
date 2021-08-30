#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "helpers.h"

void centerPrint(String str, LiquidCrystal_I2C lcd, int maxcol, int row) {
    lcd.setCursor((maxcol - str.length()) / 2, row); //center string
    lcd.print(str);
};