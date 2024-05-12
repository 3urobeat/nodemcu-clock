/*
 * File: titlebar.cpp
 * Project: nodemcu-clock
 * Created Date: 2024-05-12 09:52:01
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-12 09:53:45
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


bool miniClockActive;  // Tracks if the mini clock is currently shown
bool loadingActive = false; // Will be set to true when a loading icon is currently shown. Checking this var is faster than updating the screen every loop

char miniClockResult[6];


/**
 * Updates the title bar
 * @param title Title of the page to show
 * @param hideMiniClock If the mini clock should not be shown
 */
void updateTitleBar(const char *title, bool hideMiniClock)
{
    lcd.setCursor(0, 0);
    lcd.print(title); // TODO: Cut and append "..." if too long

    // Display mini clock unless page doesn't want us to
    if (!hideMiniClock) miniClock();

    miniClockActive = !hideMiniClock;
}


/**
 * Helper function that displays a loading icon in the first row beside the mini clock. The next loop iteration deletes the icon again.
 * @param clearIcon If true, the position of the loading icon will be cleared when a loading icon is currently shown. Default: false
 */
void indicateLoading(bool clearIcon)
{
    if ((clearIcon && !loadingActive) || (!clearIcon && loadingActive)) return; // Prevent unnecessary calculations below when call is redundant

    // Calculate position, depending on if the mini clock is currently active
    uint8_t pos = Config::maxcol - strlen(Config::miniClockFormat) - 2;

    if (!miniClockActive) pos = Config::maxcol - 1;

    // Set cursor to position and write loading icon char or whitespace to screen
    lcd.setCursor(pos, 0);

    if (!clearIcon) {
        lcd.write((byte) 2);
        loadingActive = true;
    } else {
        lcd.write(' ');
        loadingActive = false;
    }
}


/**
 * Displays a mini clock in the top right formatted using Config::miniClockFormat when hideMiniClock is false
 * Call this function on every loop() iteration
 * @param hideMiniClock Ignores call if true, some pages (for example clock) don't need another clock in the corner
 */
void miniClock()
{
    getTime(miniClockResult, sizeof(miniClockResult), Config::miniClockFormat);

    lcd.setCursor(Config::maxcol - strlen(Config::miniClockFormat), 0); //set cursor to the very right of the first line
    lcd.print(miniClockResult);
}
