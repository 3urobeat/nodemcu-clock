/*
 * File: time.cpp
 * Project: nodemcu-clock
 * Created Date: 03.09.2021 10:06:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 23:04:27
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


/**
 * Gets the current date, formats it using Config::dateformat and writes it to dest
 * @param dest Destination char array
 * @param destLen Length of dest char array (excluding null byte)
 * @param timeClient NTPClient object created in main.cpp
 */
void getDate(char *dest, uint16_t destLen, NTPClient timeClient)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, Config::dateformat, strlen(Config::dateformat));

    strrpl(dest, "dd", lcd.toFixedLengthNumber(buf, day(epoch), 2), destLen);
    strrpl(dest, "mm", lcd.toFixedLengthNumber(buf, month(epoch), 2), destLen);
    strrpl(dest, "yyyy", lcd.toFixedLengthNumber(buf, year(epoch), 4), destLen); // Call toFixedLengthNumber() even for year as it does the itoa conversion for us (otherwise unnecessary unless someone builds a time machine)
}


/**
 * Gets the current time, formats it using format and writes it to dest
 * @param dest Destination char array
 * @param destLen Length of dest char array (excluding null byte)
 * @param timeClient NTPClient object created in main.cpp
 * @param format Pointer to char array containing format
 */
void getTime(char *dest, uint16_t destLen, NTPClient timeClient, const char *format)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, format, strlen(format));

    strrpl(dest, "hh", lcd.toFixedLengthNumber(buf, hour(epoch), 2), destLen);
    strrpl(dest, "mm", lcd.toFixedLengthNumber(buf, minute(epoch), 2), destLen);
    strrpl(dest, "ss", lcd.toFixedLengthNumber(buf, second(epoch), 2), destLen);
}