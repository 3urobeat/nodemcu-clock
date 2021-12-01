/*
 * File: clock.cpp
 * Project: nodemcu-clock
 * Created Date: 01.09.2021 15:17:00
 * Author: 3urobeat
 * 
 * Last Modified: 01.12.2021 14:41:54
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

#include <TimeLib.h>


unsigned long lastPageMod = millis();
bool          currentmod  = false;    //in this case we only have two mods, so lets choose a boolean


void clockpage(LiquidCrystal_PCF8574 lcd, NTPClient timeClient, int timeoffset, int clockWeekdaySwitch, String dateformat, String timeformat, int maxcol)
{
    String dayNames[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" }; //structure is weird, but look at the docs: https://github.com/PaulStoffregen/Time#functionality

    //Always print the current time
    centerPrint(getTime(timeClient, timeoffset, timeformat).c_str(), 1, false);


    //Switch between date and weekday when clockWeekdaySwitch ms passed since last mod switch
    if (clockWeekdaySwitch > 0 && millis() >= lastPageMod + clockWeekdaySwitch) {
        currentmod = !currentmod;
        lastPageMod = millis();
    }

    //Show weekday if true and date if false, starts with false
    if (currentmod) {
        unsigned long epoch = timeClient.getEpochTime() + timeoffset;

        centerPrint(dayNames[weekday(epoch) - 1], 2, true);
    } else {
        centerPrint(getDate(timeClient, timeoffset, dateformat).c_str(), 2, true);
    }
}