/*
 * File: lcd.cpp
 * Project: helpers
 * Created Date: 30.08.2021 14:54:00
 * Author: 3urobeat
 * 
 * Last Modified: 28.11.2021 16:31:29
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


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