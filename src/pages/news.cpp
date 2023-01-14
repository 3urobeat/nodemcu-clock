/*
 * File: news.cpp
 * Project: nodemcu-clock
 * Created Date: 12.12.2021 21:27:54
 * Author: 3urobeat
 * 
 * Last Modified: 14.01.2023 13:20:08
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

// Store last API call timestamp & current article index
uint32_t lastRefresh;
uint8_t  currentArticle;

// These 3 are storing the current 4 news articles on the heap
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
        uint8_t oldMoveOffset = moveOffset; // Track offset before movingPrint() call so we can detect a moveOffset reset

        // Call movingPrint() to refresh article title string position
        lcd.setCursor(0, 3);
        lcd.movingPrint(titleCache[currentArticle], &moveOffset, Config::maxcol);

        // Handle special case of showuntil = 0: Detect if movingPrint() just reset moveOffset as string reached the end and force-Call nextPage()
        if (moveOffset < oldMoveOffset && Config::showuntil[currentPage] == 0) nextPage();
    }


    // Helper function to refresh newsCache, called by setup function
    void refreshCache()
    {
        // Check if user didn't provide an API key, display warning for 5 seconds and force-progress page
        if (strlen(Config::newsapitoken) == 0) {
            lcd.centerPrint("Error!", 1);
            lcd.centerPrint("No API key provided.", 3);
            delay(5000);
            nextPage();
            return;
        }

        debugMemory(F("news page: Refreshing cache"));
        
        // Display loading message so the device doesn't look like it crashed
        lcd.centerPrint("Loading...", 2, false);

        // Construct URL (128 B in stack should be fine, we only need it every 20 min so keeping it in heap might be wasted mem)
        char url[128] = "http://newsapi.org/v2/top-headlines?country="; // Use http instead of https as the SSL header takes like 20KB of heap, it's crazy
        char *p = url;

        p = mystrcat(p, country);
        p = mystrcat(p, "&pageSize=4&apiKey=");
        p = mystrcat(p, Config::newsapitoken);
        *(p) = '\0'; // Make sure there is a null char at the end

        // Create obj of our parser class and make request
        NewsJsonHandler *parser = new NewsJsonHandler(sourceCache[0], 32, pubAtCache[0], 6, titleCache[0], 256, 4);

        debugMemory(F("news page: Constructed URL and made parser object"));

        httpGetJson(url, parser);

        // Clear up memory
        delete(parser);

        // Update lastRefresh timestamp so next refresh only happens in updateInterval ms
        lastRefresh = millis();

        // Clear "Loading..."
        lcd.clearLine(2);

        debugMemory(F("news page: Refresh done"));
    }
}