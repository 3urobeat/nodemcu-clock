/*
 * File: clock.cpp
 * Project: nodemcu-clock
 * Created Date: 01.09.2021 15:17:00
 * Author: 3urobeat
 * 
 * Last Modified: 30.11.2021 21:04:30
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"
#include "helpers/helpers.h"


void clockpage(LiquidCrystal_PCF8574 lcd, NTPClient timeClient, int timeoffset, String dateformat, String timeformat, int maxcol)
{
    centerPrint(getTime(timeClient, timeoffset, timeformat).c_str(), 1, false);
    centerPrint(getDate(timeClient, timeoffset, dateformat).c_str(), 2, false);
}