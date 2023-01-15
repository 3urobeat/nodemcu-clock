/*
 * File: clock.cpp
 * Project: nodemcu-clock
 * Created Date: 01.09.2021 15:17:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 17:22:59
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"


const char *dayNames[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" }; // Structure is weird, but look at the docs: https://github.com/PaulStoffregen/Time#functionality

char timeResult[9];
char dateResult[11];

uint32_t lastPageModClock = millis();
bool     currentModClock  = false;    // Tracks pageElementSwitch, as we only have 2 mods here (date & weekday) we can use a bool


namespace clockPage
{
    /**
     * Setup the clock page
     */
    void setup()
    {
        hideMiniClock = true; // Disable miniClock for this page
    }

    /**
     * Shows the clock page and switch between date and weekday
     */
    void update()
    {
        // Always print the current time
        getTime(timeResult, timeClient, timeoffset, Config::timeformat);

        lcd.centerPrint(timeResult, 1, false);

        // Switch between date and weekday when pageElementSwitch ms passed since last mod switch
        if (Config::pageElementSwitch > 0 && millis() >= lastPageModClock + Config::pageElementSwitch) {
            currentModClock = !currentModClock;
            lastPageModClock = millis();
            lcd.clearLine(2); // Clear date/weekday line
        }

        // Show weekday if true and date if false, starts with false
        if (currentModClock) {
            unsigned long epoch = timeClient.getEpochTime() + timeoffset;

            lcd.centerPrint(dayNames[weekday(epoch) - 1], 2, false);
        } else {
            getDate(dateResult, timeClient, timeoffset);

            lcd.centerPrint(dateResult, 2, false);
        }
    }
}