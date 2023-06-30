/*
 * File: time.cpp
 * Project: nodemcu-clock
 * Created Date: 03.09.2021 10:06:00
 * Author: 3urobeat
 * 
 * Last Modified: 30.06.2023 09:47:10
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/3urobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


/**
 * Gets the current date, formats it using Config::dateformat and writes it to dest
 * @param dest Destination char array
 * @param destLen Length of dest char array
 */
void getDate(char *dest, uint16_t destLen)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5] = "";

    // Copy dateformat into dest (I previously just used memcpy but that seems to be broken on the ESP32 so I need to clear and then strcpy format into dest)
    memset(dest, '\0', destLen);
    strncpy(dest, Config::dateformat, destLen - 1);

    strrpl(dest, "dd", lcd.toFixedLengthNumber(buf, day(epoch), 2), destLen);
    strrpl(dest, "mm", lcd.toFixedLengthNumber(buf, month(epoch), 2), destLen);
    strrpl(dest, "yyyy", lcd.toFixedLengthNumber(buf, year(epoch), 4), destLen); // Call toFixedLengthNumber() even for year as it does the itoa conversion for us (otherwise unnecessary unless someone builds a time machine)
}


/**
 * Gets the current time, formats it using format and writes it to dest
 * @param dest Destination char array
 * @param destLen Length of dest char array
 * @param format Pointer to char array containing format
 */
void getTime(char *dest, uint16_t destLen, const char *format)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5] = "";

    // Copy format into dest (I previously just used memcpy but that seems to be broken on the ESP32 so I need to clear and then strcpy format into dest)
    memset(dest, '\0', destLen);
    strncpy(dest, format, destLen - 1);

    strrpl(dest, "hh", lcd.toFixedLengthNumber(buf, hour(epoch), 2), destLen);
    strrpl(dest, "mm", lcd.toFixedLengthNumber(buf, minute(epoch), 2), destLen);
    strrpl(dest, "ss", lcd.toFixedLengthNumber(buf, second(epoch), 2), destLen);
}