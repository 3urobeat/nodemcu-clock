/*
 * File: time.cpp
 * Project: nodemcu-clock
 * Created Date: 03.09.2021 10:06:00
 * Author: 3urobeat
 * 
 * Last Modified: 29.11.2021 17:37:51
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

String formatInt(int value)
{
    if (value < 10) return (String) "0" + value;
        else return (String) value;
}

String getDate(NTPClient timeClient, int timeoffset, String dateformat)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset; 

    //make copy from original value to not overwrite it
    String date = dateformat;

    date.replace("dd", formatInt(day(epoch)));
    date.replace("mm", formatInt(month(epoch)));
    date.replace("yyyy", formatInt(year(epoch)));

    return date;
}

//Provide function to help construct a mini clock that will be called by main.cpp when alwaysShowTime is true
String getTime(NTPClient timeClient, int timeoffset, String timeformat)
{
    unsigned long epoch = timeClient.getEpochTime() + timeoffset; 

    //make copy from original value to not overwrite it
    String time = timeformat;

    time.replace("hh", formatInt(hour(epoch)));
    time.replace("mm", formatInt(minute(epoch)));
    time.replace("ss", formatInt(second(epoch)));

    return time;
}