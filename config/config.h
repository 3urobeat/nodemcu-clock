/*
 * File: config.h
 * Project: nodemcu-clock
 * Created Date: 14.11.2022 19:16:25
 * Author: 3urobeat
 * 
 * Last Modified: 15.11.2022 14:32:00
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#pragma once


// Declare config variables
namespace Config 
{
    extern const int maxcol;

    extern const char *wifiSSID[2];
    extern const char *wifiPW[2];

    extern char lat[8];
    extern char lon[8];

    extern const char openweathermaptoken[33];
    extern const char newsapitoken[33];

    extern const char dateformat[11];
    extern const char timeformat[9];
    extern const char miniClockFormat[6];

    extern const char *pageOrder[3];
    extern const int   showuntil[3];
    extern const bool  alwaysShowTime;
    extern const int   clockWeekdaySwitch;

    extern const bool DEBUG;
}