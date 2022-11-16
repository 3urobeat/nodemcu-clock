/*
 * File: weather.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 17:53:00
 * Author: 3urobeat
 * 
 * Last Modified: 16.11.2022 19:33:38
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"


const uint32_t updateinterval = 600000; // 10 minutes in ms
unsigned long  lastWeatherRefresh;
char           fullStr[30]; // Store string to show


namespace weatherPage
{
    // Declare function here and define it later below to reduce clutter while being accessible from setup()
    void refreshCache();


    /**
     * Setup the weather page
     */
    void setup()
    {
        // Check if it's time to update weather cache
        if (lastWeatherRefresh == 0 || lastWeatherRefresh + updateinterval <= millis()) {
            refreshCache();
        }

        lcd.centerPrint(city, 1, false);
        lcd.centerPrint(fullStr, 2, false); // (char)223 prints the degree symbol, putting it in a string like normal would result in gibberish: https://forum.arduino.cc/t/print-degree-symbol-on-lcd/19073
    }


    /**
     * Shows the weather page
     */
    void update()
    {
        // Nothing to do here, setup() prints content already and there is nothing changing on this page
    }


    // Helper function to refresh weatherCache, called by setup function
    void refreshCache()
    {
        // Create filter so we don't get unneeded data back from the API
        StaticJsonDocument<128> filter;
        
        filter["main"]["temp"] = true;
        filter["weather"][0]["main"] = true;
        
        // Construct URL
        char requestStr[200] = "http://api.openweathermap.org/data/2.5/weather?lat=";
        char *p = requestStr;

        p = mystrcat(p, Config::lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, Config::lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, Config::openweathermaptoken);
        *(p) = '\0'; // Add null char to the end
        
        // Make the API call
        DynamicJsonDocument weatherResult(256);
        httpGetJson(requestStr, &weatherResult, filter);

        // Refresh fullStr
        itoa((int8_t) round((double) weatherResult["main"]["temp"] - 273.15), fullStr, 10); // Convert temp to string and write into fullStr

        char *fSp = fullStr;
        
        fSp = mystrcat(fSp, "°C, ");
        fSp = mystrcat(fSp, weatherResult["weather"][0]["main"]);
        *(p) = '\0'; // Add null char to the end just to be sure, I think there might already be one

        lastWeatherRefresh = millis();
    }
}