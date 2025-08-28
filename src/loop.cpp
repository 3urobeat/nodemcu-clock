/*
 * File: loop.cpp
 * Project: nodemcu-clock
 * Created Date: 2022-10-30 19:01:32
 * Author: 3urobeat
 *
 * Last Modified: 2025-08-28 21:33:34
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers/helpers.h"
#include "setupMode/setupMode.h"
#include "pages/pages.h"


int8_t   currentPage = -1; // Current page index in pageOrder array (init with  -1 to not skip page 0 on startup)
int8_t   oldPage;          // Save previous page to determine if we need to call lcd.clear() (useful if user only set one page in pageOrder to avoid blinking every showuntil seconds)
uint32_t pageSwitch;       // Save timestamp when page was switched in order to keep track of showuntil without blocking thread with delay()
uint32_t pageUpdate;       // Save timestamp when page was last updated, to check if it wants to be updated again


// Declare functions here and define it later below to reduce clutter while being accessible from loopHandler()
void nextPage();
void updateTitleBar(const char *title, bool hideMiniClock);


/**
 * Gets called on every loop() iteration and handles page loading/switching
 */
void loopHandler()
{
    // Call handleSetupMode() if setupMode is enabled
    if (setupModeEnabled) {
        setupModeLoop();
        return; // Stop further execution of "normal" mode
    }

    // Let timelib check if it is time to update the time
    timeClient.update();

    // Save currentPage
    oldPage = currentPage;

    // Show next page if showuntil time for this page passed (0 is special case where a page only self-progresses on a certain event) (or call setup function if currentPage is -1)
    if (currentPage == -1 || (Config::showuntil[currentPage] != 0 && pageSwitch + Config::showuntil[currentPage] <= millis()))
    {
        nextPage();
    }


    // Log available memory if DEBUG mode is enabled
    debug();

    // Check if backlight should be toggled
    handleBacklight();

    // Update miniClock if enabled. Run this before page update to prevent clock being hidden when page takes time to load
    if (miniClockActive) miniClock();

    // Remove loading icon again
    indicateLoading(true);


    // Get currently selected page
    void (*updateFunc)();
    int updateInterval;

    if (strcmp(Config::pageOrder[currentPage], "clock") == 0) {
        updateFunc = clockPage::update;
        updateInterval = clockPage::updateInterval;
    } else if (strcmp(Config::pageOrder[currentPage], "weather") == 0) {
        updateFunc = weatherPage::update;
        updateInterval = weatherPage::updateInterval;
    } else if (strcmp(Config::pageOrder[currentPage], "news") == 0) {
        updateFunc = newsPage::update;
        updateInterval = newsPage::updateInterval;
    } else if (strcmp(Config::pageOrder[currentPage], "spotify") == 0) {
        updateFunc = spotifyPage::update;
        updateInterval = spotifyPage::updateInterval;
    }

    // Check if page wants to be updated again and call update function
    if (updateFunc && pageUpdate + updateInterval <= millis())
    {
        pageUpdate = millis();
        updateFunc();
    }
}


/**
 * Helper function that switches to next page and calls setup function
 */
void nextPage()
{
    currentPage++;

    // Reset currentPage if we exceeded the amount of pages in the array
    if (currentPage + 1 > *(&Config::pageOrder + 1) - Config::pageOrder) currentPage = 0; // Credit for array size hack: https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/

    pageSwitch = millis(); // Update timestamp

    lcd.clear(); // Clear content of old page

    // Check if pageOrder entry for this index is empty or 0 and skip page
    if (strlen(Config::pageOrder[currentPage]) == 0 || Config::pageOrder[currentPage][0] == '0') return nextPage();

    // Construct title bar based on page metadata and call setup function for incoming page
    if (strcmp(Config::pageOrder[currentPage], "clock") == 0) {
        updateTitleBar(clockPage::title, clockPage::hideMiniClock);
        clockPage::setup();
    } else if (strcmp(Config::pageOrder[currentPage], "weather") == 0) {
        updateTitleBar(weatherPage::title, weatherPage::hideMiniClock);
        weatherPage::setup();
    } else if (strcmp(Config::pageOrder[currentPage], "news") == 0) {
        updateTitleBar(newsPage::title, newsPage::hideMiniClock);
        newsPage::setup();
    } else if (strcmp(Config::pageOrder[currentPage], "spotify") == 0) {
        updateTitleBar(spotifyPage::title, spotifyPage::hideMiniClock);
        spotifyPage::setup();
    }
}
