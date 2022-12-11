/*
 * File: news.cpp
 * Project: nodemcu-clock
 * Created Date: 12.12.2021 21:27:54
 * Author: 3urobeat
 * 
 * Last Modified: 11.12.2022 15:29:39
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"


const uint32_t updateInterval = 1200000; // 20 min in ms

uint32_t lastRefresh;
uint8_t  lastArticleShown;

uint8_t moveOffset = 0; // Track moveOffset to be able to reset it when news article changes

char sourceCache[4][32];
char pubAtCache[4][6];
char titleCache[4][256];

uint32_t lastArticleSwitch = 0;


namespace newsPage
{
    // Declare function here and define it later below to reduce clutter while being accessible from setup()
    void refreshCache();


    /**
     * Setup the news page
     */
    void setup()
    {
        // Check if updateInterval ms passed and update newsCache
        if (lastRefresh == 0 || lastRefresh + updateInterval <= millis()) refreshCache();

        // Switch to next article if first article was shown at least once
        if (lastArticleSwitch > 0) lastArticleShown++;
        lastArticleSwitch = millis();
        moveOffset = 0; // Reset moveOffset so the next article headline starts from index 0

        // Reset to first article if last has been displayed
        if (lastArticleShown >= 4) lastArticleShown = 0;

        // Show page title
        lcd.setCursor(0, 0);
        lcd.print("News");

        // Show article source
        lcd.setCursor(0, 1);
        lcd.print(sourceCache[lastArticleShown]);

        // Show article date
        lcd.setCursor(0, 2);
        lcd.print(pubAtCache[lastArticleShown]);
    }


    /**
     * Shows the news page
     */
    void update()
    {
        // Call movingPrint() to refresh article title string position
        lcd.setCursor(0, 3);
        lcd.movingPrint(titleCache[lastArticleShown], &moveOffset, Config::maxcol);
    }


    // Helper function to refresh newsCache, called by setup function // TODO: Can this function be optimized memory wise?
    void refreshCache()
    {
        // Display loading message as the display otherwise is just empty, leaving user unsure if the device crashed
        lcd.setCursor(0, 0);
        lcd.print("News");
        lcd.centerPrint("Loading...", 2, false);

        // Create filter to reduce memory load
        StaticJsonDocument<128> filter;
        
        JsonObject filter_articles_0 = filter["articles"].createNestedObject(); // Makes creating a filter 10 times easier: https://arduinojson.org/v6/assistant/
        filter_articles_0["source"]["name"] = true;
        filter_articles_0["title"] = true;
        filter_articles_0["publishedAt"] = true;

        // Build request url
        char fullstr[200] = "https://newsapi.org/v2/top-headlines?country=";
        char *p = fullstr;

        p = mystrcat(p, country);
        p = mystrcat(p, "&pageSize=4&apiKey=");
        p = mystrcat(p, Config::newsapitoken);
        *(p) = '\0'; // Add null char to the end

        //make request
        DynamicJsonDocument newsCache(2048);

        httpsGetJson(fullstr, &newsCache, filter);

        // Disabled for now as it causes an error
        //if (strcmp(newsCache[0], "connection failed") != 0) //if the request didn't succeed then let it ping the api again on the next iteration
        lastRefresh = millis();

        for (uint8_t i = 0; i < newsCache["articles"].size(); i++) {

            // Copy source name into cache
            memset(sourceCache[i], 0, sizeof(sourceCache[i]) - 1); // Clear old content just to make sure there is no gibberish left that could f us in the ass later
            strncpy(sourceCache[i], newsCache["articles"][i]["source"]["name"], sizeof(sourceCache[i]) - 1);
            

            // ------ Process publishedAt ------
            char pubAtBuf[10];

            // Pseudo: substring(dest, src pointer + offset, length)          | Expected result: 2021-12-15T07:15:04Z -> 07:15
            strncpy(pubAtBuf, newsCache["articles"][i]["publishedAt"].as<const char*>() + strcspn(newsCache["articles"][i]["publishedAt"].as<const char*>(), "T") + 1, 8);

            // Convert hours and minutes in String to seconds
            unsigned long inLocalSeconds;
            char hours[3] = { pubAtBuf[0], pubAtBuf[1], '\0' };
            char minutes[3] = { pubAtBuf[3], pubAtBuf[4], '\0' };

            inLocalSeconds = (atoi(hours) * 3600) + (atoi(minutes) * 60) + timeoffset; // Add timeoffset

            // Format time using miniClockFormat and reuse pubAtBuf
            strncpy(pubAtBuf, Config::miniClockFormat, 6);

            char buf[3];
            
            strrpl(pubAtBuf, "hh", lcd.toFixedLengthNumber(buf, hour(inLocalSeconds), 2));
            strrpl(pubAtBuf, "mm", lcd.toFixedLengthNumber(buf, minute(inLocalSeconds), 2));

            // Write new time string
            memset(pubAtCache[i], 0, sizeof(pubAtCache[i]) - 1); // Clear old content just to make sure there is no gibberish left that could f us in the ass later
            strncpy(pubAtCache[i], pubAtBuf, 5);

        
            // Add two spaces to the front and back of the string
            memset(titleCache[i], 0, sizeof(titleCache[i]) - 1); // Clear old content just to make sure there is no gibberish left that could f us in the ass later
            strcpy(titleCache[i], "  "); // Copy first part, cat the other parts

            char *tp = titleCache[i];
            
            tp = mystrcat(tp, newsCache["articles"][i]["title"]);
            tp = mystrcat(tp, "  ");
            *(tp) = '\0'; // Add null char to the end
        }

        // Clear "Loading..."
        lcd.clearLine(2);
    }
}