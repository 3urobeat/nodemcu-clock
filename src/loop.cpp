/*
 * File: loop.cpp
 * Project: nodemcu-clock
 * Created Date: 30.10.2022 19:01:32
 * Author: 3urobeat
 * 
 * Last Modified: 15.11.2022 13:22:26
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "helpers/helpers.h"
#include "pages/pages.h"


int           currentPage;                                         // Current page index in pageOrder array
int           oldPage;                                             // Save previous page to determine if we need to call lcd.clear() (useful if user only set one page in pageOrder to avoid blinking every showuntil seconds)
unsigned long pageUpdate = millis() + Config::showuntil[0] + 5000; // Save timestamp when page was updated in order to keep track of showuntil without blocking thread with delay() (init with +showuntil so first page is shown on startup, +5sec as buffer)
bool          hideMiniClock = false;                               // Will be set to true when clockpage is active


// Helper function to quickly debug memory usage
void debugMemory() 
{
    Serial.print("getFreeHeap(): ");
    Serial.println(ESP.getFreeHeap());
    Serial.print("getFreeContStack(): ");
    Serial.println(ESP.getFreeContStack());
}


/**
 * Gets called on every loop() iteration and handles page loading/switching
 */
void loopHandler()
{
    // Let timelib check if it is time to update the time
    timeClient.update();

    // Save currentPage
    oldPage = currentPage;

    // Update currentPage if showuntil time for this page passed
    if (pageUpdate + Config::showuntil[currentPage] <= millis())
    {
        currentPage = currentPage + 1;

        // Reset currentPage if we exceeded the amount of pages in the array
        if (currentPage + 1 > *(&Config::pageOrder + 1) - Config::pageOrder) currentPage = 0; // Credit for array size hack: https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/

        hideMiniClock = false;    // Default
        pageUpdate    = millis(); // Update timestamp

        if (oldPage != currentPage) lcd.clear();
    }

    // Call debugMemory function if DEBUG is enabled
    if (Config::DEBUG) {
        debugMemory();
    }

    if (strcmp(Config::pageOrder[currentPage], "clock") == 0) { // Why strcmp? https://stackoverflow.com/questions/2603039/warning-comparison-with-string-literals-results-in-unspecified-behaviour
        
        clockpage(timeClient);
        hideMiniClock = true;

    } else if (strcmp(Config::pageOrder[currentPage], "weather") == 0) {

        weatherpage();

    } else if (strcmp(Config::pageOrder[currentPage], "news") == 0) {

        newspage();

    }

    // Update miniClock if enabled
    if (Config::alwaysShowTime) miniClock(hideMiniClock);
}