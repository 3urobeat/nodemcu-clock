/*
 * File: news.cpp
 * Project: nodemcu-clock
 * Created Date: 12.12.2021 21:27:54
 * Author: 3urobeat
 * 
 * Last Modified: 09.02.2022 17:11:17
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <ArduinoJson.h>
#include <TimeLib.h>

#include "helpers/helpers.h"

const int  updateInterval = 1200000; //20 min in ms

unsigned int lastRefresh;
unsigned int lastArticleShown;

char sourceCache[4][32];
char pubAtCache[4][6];
char titleCache[4][256];

unsigned int lastArticleSwitch = millis();


void newspage(const char *newsapitoken, int showuntil, const char *country, int timeoffset, const char *miniClockFormat)
{
    //Check if updateInterval ms passed and update newsCache
    if (lastRefresh == 0 || lastRefresh + updateInterval <= millis()) {
        //Display loading message as the display otherwise is just empty, leaving user unsure if the device crashed
        lcdSetCursor(0, 0);
        lcdPrint("News");
        centerPrint("Loading...", 2, false);

        //create filter to reduce memory load
        StaticJsonDocument<128> filter;
        
        JsonObject filter_articles_0 = filter["articles"].createNestedObject(); //makes creating a filter 10 times easier: https://arduinojson.org/v6/assistant/
        filter_articles_0["source"]["name"] = true;
        filter_articles_0["title"] = true;
        filter_articles_0["publishedAt"] = true;

        //build request url
        char fullstr[200] = "https://newsapi.org/v2/top-headlines?country=";
        char *p = fullstr;

        p = mystrcat(p, country);
        p = mystrcat(p, "&pageSize=4&apiKey=");
        p = mystrcat(p, newsapitoken);
        *(p) = '\0'; //add null char to the end

        //make request
        DynamicJsonDocument newsCache(2048);

        httpsGetJson(fullstr, &newsCache, filter);

        //Disabled for now as it causes an error
        //if (strcmp(newsCache[0], "connection failed") != 0) //if the request didn't succeed then let it ping the api again on the next iteration
        lastRefresh = millis();

        for (unsigned int i = 0; i < newsCache["articles"].size(); i++) {

            //Copy source name into cache
            memset(sourceCache[i], 0, sizeof sourceCache[i]); //clear old content just to make sure there is no gibberish left that could f us in the ass later
            strncpy(sourceCache[i], newsCache["articles"][i]["source"]["name"], 31);
            

            // ------ Process publishedAt ------
            char pubAtBuf[10];

            //Pseudo: substring(dest, src pointer + offset, length)          | Expected result: 2021-12-15T07:15:04Z -> 07:15
            strncpy(pubAtBuf, newsCache["articles"][i]["publishedAt"].as<const char*>() + strcspn(newsCache["articles"][i]["publishedAt"].as<const char*>(), "T") + 1, 8);

            //Convert hours and minutes in String to seconds
            unsigned long inLocalSeconds;
            char hours[3] = { pubAtBuf[0], pubAtBuf[1], '\0' };
            char minutes[3] = { pubAtBuf[3], pubAtBuf[4], '\0' };

            inLocalSeconds = (atoi(hours) * 3600) + (atoi(minutes) * 60) + timeoffset; //add timeoffset

            //Format time using miniClockFormat and reuse pubAtBuf
            strncpy(pubAtBuf, miniClockFormat, 5);

            char buf[3];
            
            strrpl(pubAtBuf, "hh", formatInt(buf, hour(inLocalSeconds)));
            strrpl(pubAtBuf, "mm", formatInt(buf, minute(inLocalSeconds)));

            //write new time string
            memset(pubAtCache[i], 0, sizeof pubAtCache[i]); //clear old content just to make sure there is no gibberish left that could f us in the ass later
            strncpy(pubAtCache[i], pubAtBuf, 5);
  
           
            //Add two spaces to the front and back of the string
            memset(titleCache[i], 0, sizeof titleCache[i]); //clear old content just to make sure there is no gibberish left that could f us in the ass later
            strcpy(titleCache[i], "  "); //copy first part, cat the other parts

            char *tp = titleCache[i];
            
            tp = mystrcat(tp, newsCache["articles"][i]["title"]);
            tp = mystrcat(tp, "  ");
            *(tp) = '\0'; //add null char to the end
        }
    }

    //Check if the page just changed to this one and show the next article
    if (lastArticleSwitch + showuntil < millis()) {
        lastArticleShown++;
        lastArticleSwitch = millis();

        if (lastArticleShown >= 4) lastArticleShown = 0; //reset

        //Show page title
        lcdSetCursor(0, 0);
        lcdPrint("News");

        //Show article source
        clearLine(maxcol, 1);

        lcdSetCursor(0, 1);
        lcdPrint(sourceCache[lastArticleShown]);

        //Show article date
        lcdSetCursor(0, 2);
        lcdPrint(pubAtCache[lastArticleShown]);
    }

    //Call the movingPrint method to refresh string position
    movingPrint(titleCache[lastArticleShown], 3, false); //add two spaces to make animation start and end look smoother
}