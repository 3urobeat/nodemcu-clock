/*
 * File: lcd.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 14:54:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.12.2021 20:41:16
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
    if (str.length() > (unsigned int) maxcol) str = str.substring(0, maxcol); //Cut String if it is too long to prevent it overflowing to another row

    lcd.print(str);
    
    lcdContent[lcdCursorPos[1]] = str;
}


void centerPrint(String str, int row, bool callclearLine)
{
    if (callclearLine && lcdContent[row] != str) clearLine(maxcol, row); //clear the line first to avoid old characters corrupting the text when content is not the same

    //Calculate column
    int offset = maxcol - str.length();
    if (offset < 0) offset = 0; //set offset to 0 if it would be negative

    lcdSetCursor(offset / 2, row); //center string
    lcdPrint(str);
};


String       currentStr;
unsigned int moveOffset = 0;

/**
 * Prints a String that will be moved on the screen each time the method is called
 */
void movingPrint(String str, int row, bool callclearLine)
{
    if (callclearLine && currentStr != str) clearLine(maxcol, row);

    if (str.length() > (unsigned int) maxcol) { //check if we actually have to move something, if not display it as a center string
        if (currentStr != str) {
            moveOffset = 0; //reset offset if the string isn't the same anymore
            currentStr = str;
        }

        moveOffset++;
        if (moveOffset + maxcol > str.length()) moveOffset = 0; //reset if string was fully displayed

        lcdSetCursor(0, row);
        lcdPrint(str.substring(moveOffset, maxcol + moveOffset));

    } else {
        centerPrint(str, row, callclearLine);
    }
}