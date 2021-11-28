#include <iostream>
#include "helpers.h"

void clearLine(LiquidCrystal_PCF8574 lcd, int maxcol, int row)
{
    lcd.setCursor(0, row);
    lcd.print(std::string(maxcol, ' ').c_str()); //print maxcol amount of spaces to effectively clear the line
}


void centerPrint(String str, LiquidCrystal_PCF8574 lcd, int maxcol, int row, bool callclearLine)
{
    if (callclearLine) clearLine(lcd, maxcol, row); //clear the line first to avoid old characters corrupting the text

    lcd.setCursor((maxcol - str.length()) / 2, row); //center string
    lcd.print(str);
};