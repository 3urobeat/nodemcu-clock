/*
 * File: pages.h
 * Project: nodemcu-clock
 * Created Date: 01.09.2021 15:17:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.12.2021 20:16:29
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

void clockpage(NTPClient timeClient, int timeoffset, int clockWeekdaySwitch, String dateformat, String timeformat);
void weatherpage(String openweathermaptoken, String lat, String lon, String city);
void newspage(String newsapitoken, int showuntil, String country, int timeoffset, String miniClockFormat);
