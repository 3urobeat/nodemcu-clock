/*
 * File: miniClock.cpp
 * Project: nodemcu-clock
 * Created Date: 30.10.2022 18:45:33
 * Author: 3urobeat
 * 
 * Last Modified: 22.01.2023 10:24:07
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
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