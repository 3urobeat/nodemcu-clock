/*
 * File: time.cpp
 * Project: nodemcu-clock
 * Created Date: 03.09.2021 10:06:00
 * Author: 3urobeat
 * 
 * Last Modified: 16.11.2022 19:30:34
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


char *formatInt(char *buf, int16_t value)
{
    itoa(value, buf, 10); //int value to string

    if (value < 10) {
        char buf2[strlen(buf) + 2];

        //copy current value in buf into second buffer, then overwrite first buffer with "0" and add buf2 back to buf again
        strcpy(buf2, buf); //looks shit, probably is shit but I've got no other idea right now
        strcpy(buf, "0");
        strcat(buf, buf2);
    }

    return buf;
}


void getDate(char *dest, NTPClient timeClient, uint16_t timeoffset)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, Config::dateformat, strlen(Config::dateformat));

    strrpl(dest, "dd", formatInt(buf, day(epoch)));
    strrpl(dest, "mm", formatInt(buf, month(epoch)));
    strrpl(dest, "yyyy", formatInt(buf, year(epoch)));
}


// Provide function to help construct time string
void getTime(char *dest, NTPClient timeClient, uint16_t timeoffset, const char *format)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, format, strlen(format));

    strrpl(dest, "hh", formatInt(buf, hour(epoch)));
    strrpl(dest, "mm", formatInt(buf, minute(epoch)));
    strrpl(dest, "ss", formatInt(buf, second(epoch)));
}