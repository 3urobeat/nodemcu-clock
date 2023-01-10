/*
 * File: news.cpp
 * Project: nodemcu-clock
 * Created Date: 12.12.2021 21:27:54
 * Author: 3urobeat
 * 
 * Last Modified: 10.01.2023 22:15:52
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"


const uint32_t updateInterval = 1200000; // 20 min in ms // TODO: Move to config?

// Store last API call timestamp, last 
uint32_t lastRefresh;
uint8_t  currentArticle;

// These 3 are storing the current 4 news articles
char sourceCache[4][32];
char pubAtCache[4][6];
char titleCache[4][256];

uint8_t moveOffset = 0; // moveOffset for movingPrint() call that displays title


namespace newsPage
{
    // Declare function here and define it later below to reduce clutter while being accessible from setup()
    void refreshCache();


    /**
     * Setup the news page
     */
    void setup()
    {
        // Show page title
        lcd.setCursor(0, 0);
        lcd.print("News");

        // Check if updateInterval ms passed and update newsCache
        if (lastRefresh == 0 || lastRefresh + updateInterval <= millis()) refreshCache();

        // Switch to next article and reset moveOffset so next title starts at index 0
        currentArticle++;
        moveOffset = 0;

        // Reset to first article if last has been displayed
        if (currentArticle >= 4) currentArticle = 0;

        // Show article source
        lcd.setCursor(0, 1);
        lcd.print(sourceCache[currentArticle]);

        // Show article date
        lcd.setCursor(0, 2);
        lcd.print(pubAtCache[currentArticle]);
    }


    /**
     * Shows the news page
     */
    void update()
    {
        // Call movingPrint() to refresh article title string position
        lcd.setCursor(0, 3);
        lcd.movingPrint(titleCache[currentArticle], &moveOffset, Config::maxcol);
    }


    // Helper function to refresh newsCache, called by setup function
    void refreshCache()
    {
        
    }
}