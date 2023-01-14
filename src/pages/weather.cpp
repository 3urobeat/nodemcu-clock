/*
 * File: weather.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 17:53:00
 * Author: 3urobeat
 * 
 * Last Modified: 14.01.2023 13:20:56
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
uint32_t       lastWeatherRefresh;
char           weatherStr[20]; // Stores temp and weather condition


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
        lcd.centerPrint(weatherStr, 2, false); // (char)223 prints the degree symbol, putting it in a string like normal would result in gibberish: https://forum.arduino.cc/t/print-degree-symbol-on-lcd/19073
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
        // Check if user didn't provide an API key, display warning for 5 seconds and force-progress page
        if (strlen(Config::newsapitoken) == 0) {
            lcd.setCursor(0, 0);
            lcd.print("Weather");
            lcd.centerPrint("Error!", 1);
            lcd.centerPrint("No API key provided.", 3);
            delay(5000);
            nextPage();
            return;
        }
        
        debugMemory(F("weather page: Refreshing cache"));
        
        // Construct URL
        char url[128] = "http://api.openweathermap.org/data/2.5/weather?lat=";
        char *p = url;

        p = mystrcat(p, Config::lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, Config::lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, Config::openweathermaptoken);
        *(p) = '\0'; // Make sure there is a null char at the end

        // Temp array for weather condition as it is transmitted first but we need it at 2nd pos in weatherStr
        char weatherCond[16] = "";
        char temp[6] = "";

        // Create obj of our parser class and make request
        WeatherJsonHandler *parser = new WeatherJsonHandler(weatherCond, 16, temp);

        debugMemory(F("weather page: Constructed URL and made parser object"));

        httpGetJson(url, parser);

        // Clear up memory
        delete(parser);

        // Concat weatherCond & temp to weatherStr
        memset(weatherStr, 0, sizeof(weatherStr)); // Clear previous content

        char *pStr = weatherStr;
        
        pStr = mystrcat(pStr, temp);
        pStr = mystrcat(pStr, "°C, ");
        pStr = mystrcat(pStr, weatherCond);
        *(pStr) = '\0'; // Make sure there is a null char at the end

        // Refresh timestamp so the next update will be in updateinterval ms 
        lastWeatherRefresh = millis();
        
        debugMemory(F("weather page: Refresh done"));
    }
}