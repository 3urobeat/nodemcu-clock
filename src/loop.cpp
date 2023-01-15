/*
 * File: loop.cpp
 * Project: nodemcu-clock
 * Created Date: 30.10.2022 19:01:32
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 16:16:14
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "helpers/helpers.h"
#include "setupMode/setupMode.h"
#include "pages/pages.h"


int8_t        currentPage = -1;      // Current page index in pageOrder array (init with  -1 to not skip page 0 on startup)
int8_t        oldPage;               // Save previous page to determine if we need to call lcd.clear() (useful if user only set one page in pageOrder to avoid blinking every showuntil seconds)
unsigned long pageUpdate;            // Save timestamp when page was updated in order to keep track of showuntil without blocking thread with delay()
bool          hideMiniClock = false; // Will be set to true when clockpage is active

// Declare functions here and define it later below to reduce clutter while being accessible from loopHandler()
void nextPage();


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
    if (currentPage == -1 || (Config::showuntil[currentPage] != 0 && pageUpdate + Config::showuntil[currentPage] <= millis())) nextPage();

    // Log available memory if DEBUG mode is enabled
    debugMemory();

    // Call update function for current page
    if (strcmp(Config::pageOrder[currentPage], "clock") == 0) {
        clockPage::update();
    } else if (strcmp(Config::pageOrder[currentPage], "weather") == 0) {
        weatherPage::update();
    } else if (strcmp(Config::pageOrder[currentPage], "news") == 0) {
        newsPage::update();
    }

    // Update miniClock if enabled
    if (Config::alwaysShowTime) miniClock(hideMiniClock);
}


// Helper function that switches to next page and calls setup function
void nextPage()
{
    currentPage++;

    // Reset currentPage if we exceeded the amount of pages in the array
    if (currentPage + 1 > *(&Config::pageOrder + 1) - Config::pageOrder) currentPage = 0; // Credit for array size hack: https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/

    hideMiniClock = false;    // Default
    pageUpdate    = millis(); // Update timestamp

    lcd.clear(); // Clear content of old page

    // Check if pageOrder entry for this index is empty or 0 and skip page
    if (strlen(Config::pageOrder[currentPage]) == 0 || Config::pageOrder[currentPage][0] == '0') return nextPage();

    // Call setup function for incoming page
    if (strcmp(Config::pageOrder[currentPage], "clock") == 0) {
        clockPage::setup();
    } else if (strcmp(Config::pageOrder[currentPage], "weather") == 0) {
        weatherPage::setup();
    } else if (strcmp(Config::pageOrder[currentPage], "news") == 0) {
        newsPage::setup();
    }
}