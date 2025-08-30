/*
 * File: clock.cpp
 * Project: nodemcu-clock
 * Created Date: 2021-09-01 15:17:00
 * Author: 3urobeat
 *
 * Last Modified: 2025-08-30 15:41:45
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "pages.h"


const char *dayNames[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" }; // Structure is weird, but look at the docs: https://github.com/PaulStoffregen/Time#functionality

char timeResult[9];
char dateResult[11];

uint32_t lastPageModClock = millis();
bool     currentModClock  = false;    // Tracks pageElemSwitch, as we only have 2 mods here (date & weekday) we can use a bool


namespace clockPage
{
    const char *title = "";
    const int  updateInterval = 250;
    const bool hideMiniClock = true;


    /**
     * Setup the clock page
     */
    void setup()
    {

    }

    /**
     * Shows the clock page and switch between date and weekday
     */
    void update()
    {
        // Always print the current time
        getTime(timeResult, sizeof(timeResult), Config::timeformat);

        lcd.centerPrint(timeResult, 1, false);

        // Switch between date and weekday when pageElemSwitch ms passed since last mod switch
        if (Config::pageElemSwitch > 0 && millis() >= lastPageModClock + Config::pageElemSwitch) {
            currentModClock = !currentModClock;
            lastPageModClock = millis();
            lcd.clearLine(2); // Clear date/weekday line
        } else {
            return; // No switch, no need to reprint
        }

        // Show weekday if true and date if false, starts with false
        if (currentModClock) {
            unsigned long epoch = timeClient.getEpochTime() + timeoffset;
            const char *weekdayStr = dayNames[weekday(epoch) - 1];

            // Make condition fade in left to right
            int offset = lcd.calculateCenterOffset(weekdayStr);
            lcd.setCursor(offset, 2);
            lcd.fadeInPrint(weekdayStr, 25);
        } else {
            getDate(dateResult, sizeof(dateResult));

            // ...and sun rise & sun set fade in right to left
            int offset = lcd.calculateCenterOffset(dateResult) + lcd.utf8_strlen(dateResult) - 1;
            lcd.setCursor(offset, 2);
            lcd.fadeInPrint(dateResult, 25, true, offset, 2);
        }
    }
}
