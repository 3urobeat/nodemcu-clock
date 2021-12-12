/*
 * File: lcd.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 14:54:00
 * Author: 3urobeat
 * 
 * Last Modified: 12.12.2021 22:48:21
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
#include "main.h"


void clearLine(int maxcol, int row)
{
    lcdSetCursor(0, row);
    lcdPrint(std::string(maxcol, ' ').c_str()); //print maxcol amount of spaces to effectively clear the line
}


//Custom setCursor function to be able to track cursor position
void lcdSetCursor(int col, int row)
{
    lcd.setCursor(col, row);

    lcdCursorPos[0] = col;
    lcdCursorPos[1] = row;
}


//Custom print function to be able to track content of display
void lcdPrint(String str)
{
    lcd.print(str);
    
    lcdContent[lcdCursorPos[1]] = str;
}


void centerPrint(String str, int row, bool callclearLine)
{
    if (callclearLine && lcdContent[row] != str) clearLine(maxcol, row); //clear the line first to avoid old characters corrupting the text when content is not the same

    //Calculate column
    int offset = maxcol - str.length();
    if (offset < 0) offset = 0; //set offset to 0 if it would be negative

    lcd.setCursor(offset / 2, row); //center string
    lcd.print(str);

    lcdContent[row] = str;
};