/*
 * File: news.cpp
 * Project: nodemcu-clock
 * Created Date: 12.12.2021 21:27:54
 * Author: 3urobeat
 * 
 * Last Modified: 15.12.2021 21:39:29
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

int updateInterval = 1200000; //20 min in ms

DynamicJsonDocument newsCache(1536);
unsigned int        lastRefresh;
unsigned int        lastArticleShown;

unsigned int        lastArticleSwitch = millis();


void newspage(String newsapitoken, int showuntil, String country, int timeoffset, String miniClockFormat)
{
    //Check if updateInterval ms passed and update newsCache
    if (lastRefresh == 0 || lastRefresh + updateInterval <= millis()) {
        StaticJsonDocument<128> filter; //create filter to reduce memory load
        
        JsonObject filter_articles_0 = filter["articles"].createNestedObject(); //makes creating a filter 10 times easier: https://arduinojson.org/v6/assistant/
        filter_articles_0["source"]["name"] = true;
        filter_articles_0["title"] = true;
        filter_articles_0["publishedAt"] = true;


        httpsGetJson("https://newsapi.org/v2/top-headlines?country=" + country + "&pageSize=4" + "&apiKey=" + newsapitoken, &newsCache, filter);

        if (newsCache[0] != "https error (-1): connection failed") lastRefresh = millis(); //if the request didn't succeed then let it ping the api again on the next iteration
            else newsCache["articles"][lastArticleShown]["title"] = "Error getting news!";


        for (unsigned int i = 0; i < newsCache["articles"].size(); i++) {

            //Remove date from time
            String pubAt = newsCache["articles"][i]["publishedAt"];
            pubAt = pubAt.substring(pubAt.indexOf("T") + 1, pubAt.indexOf("Z") - 2); //2021-12-15T07:15:04Z -> 07:15

            //Convert hours and minutes in String to seconds
            unsigned long inLocalSeconds;
            String hours   = (String) pubAt.charAt(0) + (String) pubAt.charAt(1);
            String minutes = (String) pubAt.charAt(3) + (String) pubAt.charAt(4);

            inLocalSeconds = (hours.toInt() * 3600) + (minutes.toInt() * 60) + timeoffset; //add timeoffset

            //Format time using miniClockFormat
            String time = miniClockFormat;
            time.replace("hh", formatInt(hour(inLocalSeconds)));
            time.replace("mm", formatInt(minute(inLocalSeconds)));

            newsCache["articles"][i]["publishedAt"] = time;


            //Remove article source from the end of the title
            String source = newsCache["articles"][i]["source"]["name"];
            String title  = newsCache["articles"][i]["title"];
            
            //Title looks like this "title text - source name" which sucks on a screen with limited size when I already show the source directly above
            newsCache["articles"][i]["title"] = title.substring(0, title.indexOf(" - ", title.length() - source.length() - 7)); //Search for " - ", starting from the end, minus the source length and some characters. This should avoid accidentaly finding a minus in the actual title
        }
    }

    //Check if the page just changed to this one and show the next article
    if (lastArticleSwitch + showuntil < millis()) {
        lastArticleShown++;
        lastArticleSwitch = millis();

        if (lastArticleShown > newsCache["articles"].size() - 1) lastArticleShown = 0; //reset

        //Show page title
        lcdSetCursor(0, 0);
        lcdPrint("News");

        //Show article source
        lcdSetCursor(0, 1);
        lcdPrint(newsCache["articles"][lastArticleShown]["source"]["name"]);

        //Show article date
        lcdSetCursor(0, 2);
        lcdPrint(newsCache["articles"][lastArticleShown]["publishedAt"]);
    }

    //Call the movingPrint method to refresh string position
    movingPrint("  " + (String) newsCache["articles"][lastArticleShown]["title"] + "  ", 3, false); //add two spaces to make animation start and end look smoother
}