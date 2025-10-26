/*
 * File: news.cpp
 * Project: nodemcu-clock
 * Created Date: 2021-12-12 21:27:54
 * Author: 3urobeat
 *
 * Last Modified: 2025-10-25 22:18:43
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "pages.h"


const uint32_t updateIntervalNews = 1200000; // 20 min in ms

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
    const char *title = "News";
    const int  updateInterval = 250;
    const bool hideMiniClock = false;


    // Declare function here and define it later below to reduce clutter while being accessible from setup()
    bool refreshCache();


    /**
     * Setup the news page
     */
    void setup()
    {
        // Check if updateIntervalNews ms passed and update newsCache
        if (lastRefresh == 0 || lastRefresh + updateIntervalNews <= millis()) {
            bool refreshSuccess = refreshCache();

            if (!refreshSuccess) { // Check if request succeeded
                nextPage();        // Skip this page for now, the next time this page is loaded it will re-attempt to refresh all titles
                return;
            }
        }


        // Switch to next article and reset moveOffset so next title starts at index 0
        currentArticle++;
        moveOffset = 0;

        if (currentArticle >= 4) currentArticle = 0; // Rollover if index exceeds size


        // Skip ahead if this titleCache entry is empty. This should *theoretically* never softlock since refreshCache() checks for failed requests
        while (strlen(titleCache[currentArticle]) == 0) {
            debug(F("newsPage: Empty titleCache entry detected, skipping ahead to next entry!"));

            currentArticle++;
            if (currentArticle >= 4) currentArticle = 0;
        }


        // Show article source
        lcd.setCursor(0, 1);
        lcd.print(sourceCache[currentArticle]);

        // Show article date
        lcd.setCursor(0, 2);
        lcd.print("Published at ");
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


    /**
     * Helper function to refresh newsCache every updateIntervalNews ms, called by setup function. Returns true on success, false otherwise.
     */
    bool refreshCache()
    {
        // Check if user didn't provide an API key, display warning for 5 seconds and force-progress page
        if (strlen(Config::newsapitoken) == 0) {
            lcd.centerPrint("Error!", 1);
            lcd.centerPrint("No API key provided.", 3);
            delay(5000);
            return false;
        }

        debug(F("news page: Refreshing cache"));

        // Display loading message so the device doesn't look like it crashed
        lcd.centerPrint("Loading...", 2, false);

        // Clear titleCache
        memset(titleCache, 0, sizeof(titleCache));


        // --------------- Get news sources for the user's country ---------------

        // Construct URL (should be fine on stack, we only need it every 20 min so keeping it in heap might be wasted mem)
        char path[192] = "/v2/top-headlines/sources?country="; // Use http instead of https as the SSL header takes like 20KB of heap, it's crazy
        char *p = path;

        p = mystrcat(p, country);
        p = mystrcat(p, "&apiKey=");
        p = mystrcat(p, Config::newsapitoken);
        *(p) = '\0'; // Make sure there is a null char at the end

        // Create obj of our parser class and make request
        char sourcesStr[64] = "";
        NewsSourcesJsonHandler *srcParser = new NewsSourcesJsonHandler(sourcesStr, 64);

        debug(F("news page: Constructed sources URL and made parser object"));

        httpGetJson("newsapi.org", path, 80, srcParser);

        // Clear up memory
        delete(srcParser);


        // --------------- Get current news from those sources ---------------

        // Construct URL, reuse path
        strcpy(path, "/v2/top-headlines?sources="); // Use http instead of https as the SSL header takes like 20KB of heap, it's crazy
        p = path;

        p = mystrcat(p, sourcesStr);
        p = mystrcat(p, "&pageSize=20&apiKey=");
        p = mystrcat(p, Config::newsapitoken);
        *(p) = '\0'; // Make sure there is a null char at the end

        // Get 4 random articles, newsapi isn't great at mixing various sources, our 4 articles will otherwise likely all be from one
        uint8_t articleIds[4] = { (uint8_t) random(0, 5), (uint8_t) random(6, 10), (uint8_t) random(11, 15), (uint8_t) random(16, 20) };

        // Create obj of our parser class and make request
        NewsJsonHandler *parser = new NewsJsonHandler(sourceCache[0], 32, pubAtCache[0], 6, titleCache[0], 256, 4, articleIds);

        debug(F("news page: Constructed URL and made parser object"));

        httpGetJson("newsapi.org", path, 80, parser);

        // Clear up memory
        delete(parser);


        // Check if all titles are !null
        bool allPagesEmpty = true;

        for (int i = 0; i < 4; i++) {
            if (strlen(titleCache[i]) > 0) allPagesEmpty = false; // Set var to false if at least one page succeeded to load
        }

        if (allPagesEmpty) {
            lcd.centerPrint("Failed to refresh!", 2);
            delay(2500);
            return false;
        }


        // Update lastRefresh timestamp so next refresh only happens in updateIntervalNews ms
        lastRefresh = millis();

        // Clear "Loading..."
        lcd.clearLine(2);

        debug(F("news page: Refresh done"));
        return true;
    }
}
