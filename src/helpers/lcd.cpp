/*
 * File: lcd.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 14:54:00
 * Author: 3urobeat
 * 
 * Last Modified: 09.02.2022 13:18:45
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <string.h>
#include <iostream>
#include "helpers.h"


void clearLine(int maxcol, int row)
{
    lcdSetCursor(0, row);

    //print maxcol amount of spaces to effectively clear the line
    char str[maxcol + 1];

    for (int i = 0; i < maxcol; i++) {
        str[i] = ' ';
    }

    str[maxcol] = '\0';

    lcdPrint(str);
}


//Custom setCursor function to be able to track cursor position
void lcdSetCursor(int col, int row)
{
    lcd.setCursor(col, row);

    lcdCursorPos[0] = col;
    lcdCursorPos[1] = row;
}


//Custom print function to be able to track content of display
void lcdPrint(const char *str)
{
    //Either print directly or cut string and then print the shortened string if it is wider than the display
    if (strlen(str) <= (unsigned int) maxcol) {
        lcd.print(str);

        strncpy(lcdContent[lcdCursorPos[1]], str, maxcol);
    } else {
        char temp[maxcol + 1]; // +1 for the null char
        strncpy(temp, str, maxcol); //Cut String if it is too long to prevent it overflowing to another row
        lcd.print(temp);

        strncpy(lcdContent[lcdCursorPos[1]], temp, maxcol);
    }
}


void centerPrint(const char *str, int row, bool callclearLine)
{
    if (callclearLine && lcdContent[row] != str) clearLine(maxcol, row); //clear the line first to avoid old characters corrupting the text when content is not the same

    //Calculate column
    int offset = maxcol - strlen(str);
    if (offset < 0) offset = 0; //set offset to 0 if it would be negative

    lcdSetCursor(offset / 2, row); //center string
    lcdPrint(str);
};


char         currentStr[256];
unsigned int moveOffset = 0;

/**
 * Prints a String that will be moved on the screen each time the method is called (max 256 chars!)
 */
void movingPrint(const char *str, int row, bool callclearLine)
{
    if (callclearLine && strcmp(currentStr, str) != 0) clearLine(maxcol, row);

    if (strlen(str) > (unsigned int) maxcol) { //check if we actually have to move something, if not display it as a center string
        if (strcmp(currentStr, str) != 0) {
            moveOffset = 0; //reset offset if the string isn't the same anymore

            memset(currentStr, 0, sizeof currentStr);
            strncpy(currentStr, str, 255); //cut after 255 chars
        }

        if (moveOffset + maxcol > strlen(str)) moveOffset = 0; //reset if string was fully displayed

        char temp[maxcol + 1] = "";
        strncpy(temp, str + moveOffset, maxcol); //substring to current offset

        lcdSetCursor(0, row);
        lcdPrint(temp);

        moveOffset++;
    } else {
        centerPrint(str, row, callclearLine);
    }
}