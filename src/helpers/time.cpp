/*
 * File: time.cpp
 * Project: nodemcu-clock
 * Created Date: 03.09.2021 10:06:00
 * Author: 3urobeat
 * 
 * Last Modified: 31.12.2021 15:41:29
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <NTPClient.h>
#include <TimeLib.h>
#include <helpers/helpers.h>

char *formatInt(char *buf, int value)
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

void getDate(char *dest, NTPClient timeClient, int timeoffset, const char *dateformat)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, dateformat, strlen(dateformat));

    strrpl(dest, "dd", formatInt(buf, day(epoch)));
    strrpl(dest, "mm", formatInt(buf, month(epoch)));
    strrpl(dest, "yyyy", formatInt(buf, year(epoch)));
}

//Provide function to help construct a mini clock that will be called by main.cpp when alwaysShowTime is true
void getTime(char *dest, NTPClient timeClient, int timeoffset, const char *timeformat)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset;

    char buf[5];

    //copy dateformat into dest
    memcpy(dest, timeformat, strlen(timeformat));

    strrpl(dest, "hh", formatInt(buf, hour(epoch)));
    strrpl(dest, "mm", formatInt(buf, minute(epoch)));
    strrpl(dest, "ss", formatInt(buf, second(epoch)));
}