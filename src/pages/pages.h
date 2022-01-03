/*
 * File: pages.h
 * Project: nodemcu-clock
 * Created Date: 01.09.2021 15:17:00
 * Author: 3urobeat
 * 
 * Last Modified: 27.12.2021 16:09:37
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <NoiascaHW/lcd_PCF8574.h>
#include <NTPClient.h>
#include <iostream>

void clockpage(NTPClient timeClient, int timeoffset, int clockWeekdaySwitch, const char *dateformat, const char *timeformat);
void weatherpage(const char *openweathermaptoken, const char *lat, const char *lon, const char *city);
void newspage(const char *newsapitoken, int showuntil, const char *country, int timeoffset, const char *miniClockFormat);
