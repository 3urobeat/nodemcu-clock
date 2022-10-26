/*
 * File: pages.h
 * Project: nodemcu-clock
 * Created Date: 01.09.2021 15:17:00
 * Author: 3urobeat
 * 
 * Last Modified: 26.10.2022 13:51:26
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <Wire.h>
#include <NTPClient.h>
#include <iostream>

void clockpage(NTPClient timeClient, int timeoffset, int clockWeekdaySwitch, const char *dateformat, const char *timeformat);
void weatherpage(const char *openweathermaptoken, const char *lat, const char *lon, const char *city);
void newspage(const char *newsapitoken, int showuntil, const char *country, int timeoffset, const char *miniClockFormat);
