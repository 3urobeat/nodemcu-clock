/*
 * File: miniClock.cpp
 * Project: nodemcu-clock
 * Created Date: 30.10.2022 18:45:33
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 23:03:13
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


char miniClockResult[6];


/**
 * Displays a mini clock in the top right formatted using Config::miniClockFormat when hideMiniClock is false
 * Call this function on every loop() iteration
 * @param hideMiniClock Ignores call if true, some pages (for example clock) don't need another clock in the corner
 */
void miniClock(bool hideMiniClock)
{
    if (!hideMiniClock)
    {
        getTime(miniClockResult, sizeof(miniClockResult) - 1, timeClient, Config::miniClockFormat);

        lcd.setCursor(Config::maxcol - strlen(Config::miniClockFormat), 0); //set cursor to the very right of the first line
        lcd.print(miniClockResult);
    }
}