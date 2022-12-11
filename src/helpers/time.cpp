/*
 * File: time.cpp
 * Project: nodemcu-clock
 * Created Date: 03.09.2021 10:06:00
 * Author: 3urobeat
 * 
 * Last Modified: 11.12.2022 15:30:20
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


void getDate(char *dest, NTPClient timeClient, uint16_t timeoffset)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, Config::dateformat, strlen(Config::dateformat));

    strrpl(dest, "dd", lcd.toFixedLengthNumber(buf, day(epoch), 2));
    strrpl(dest, "mm", lcd.toFixedLengthNumber(buf, month(epoch), 2));
    strrpl(dest, "yyyy", lcd.toFixedLengthNumber(buf, year(epoch), 4)); // Call toFixedLengthNumber() even for year as it does the itoa conversion for us (otherwise unnecessary unless someone builds a time machine)
}


// Provide function to help construct time string
void getTime(char *dest, NTPClient timeClient, uint16_t timeoffset, const char *format)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, format, strlen(format));

    strrpl(dest, "hh", lcd.toFixedLengthNumber(buf, hour(epoch), 2));
    strrpl(dest, "mm", lcd.toFixedLengthNumber(buf, minute(epoch), 2));
    strrpl(dest, "ss", lcd.toFixedLengthNumber(buf, second(epoch), 2));
}