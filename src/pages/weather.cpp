/*
 * File: weather.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 17:53:00
 * Author: 3urobeat
 * 
 * Last Modified: 17.01.2023 23:41:58
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"


const uint32_t updateIntervalWeather = 1200000; // 20 minutes in ms
uint32_t       lastWeatherRefresh;

char           tempCondStr[20];   // Stores temp and weather condition
char           sunRiseSetStr[20]; // Stores sunrise and sunset time

uint32_t lastPageModWeather = millis();
bool     currentModWeather  = false;    // Tracks pageElementSwitch, as we only have 2 mods here (sunrise & sunset) we can use a bool


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
        if (lastWeatherRefresh == 0 || lastWeatherRefresh + updateIntervalWeather <= millis()) {
            refreshCache();
        }

        lcd.centerPrint(city, 1, false);
    }


    /**
     * Shows the weather page
     */
    void update()
    {
        // Switch between temp & cond and sunrise & sunset when pageElementSwitch ms passed since last mod switch
        if (Config::pageElementSwitch > 0 && millis() >= lastPageModWeather + Config::pageElementSwitch) {
            currentModWeather  = !currentModWeather;
            lastPageModWeather = millis();
            lcd.clearLine(2); // Clear temp & cond line
        }

        // Show sunrise & sunset if true and temp & cond if false, starts with false
        if (currentModWeather) {
            lcd.centerPrint(tempCondStr, 2, false);
        } else {
            lcd.centerPrint(sunRiseSetStr, 2, false);

            // Display arrow up and down arrows at the correct locations, there are currently just placeholders
            lcd.setCursor(3, 2);
            lcd.write((byte) 0);

            lcd.setCursor(10, 2);
            lcd.write((byte) 1);
        }
    }


    /**
     * Helper function to refresh newsCache every updateIntervalNews ms, called by setup function
     */
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
        
        debug(F("weather page: Refreshing cache"));

        // Display page name and loading message so the device doesn't look like it crashed
        lcd.setCursor(0, 0);
        lcd.print("Weather");
        lcd.centerPrint("Loading...", 2, false);
        

        // Construct URL
        char url[128] = "http://api.openweathermap.org/data/2.5/weather?lat=";
        char *p = url;

        p = mystrcat(p, Config::lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, Config::lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, Config::openweathermaptoken);
        *(p) = '\0'; // Make sure there is a null char at the end


        // Temp array for weather condition as it is transmitted first but we need it at 2nd pos in tempCondStr
        char weatherCond[16] = "";
        char temp[6] = "";

        // Temp values for sunrise and sunset
        uint32_t sunrise = 0;
        uint32_t sunset = 0;


        // Create obj of our parser class and make request
        WeatherJsonHandler *parser = new WeatherJsonHandler(weatherCond, 16, temp, &sunrise, &sunset);

        debug(F("weather page: Constructed URL and made parser object"));

        httpGetJson(url, parser);

        // Clear up memory
        delete(parser);


        // Concat weatherCond & temp to tempCondStr
        memset(tempCondStr, 0, sizeof(tempCondStr)); // Clear previous content

        char *tempP = tempCondStr;
        
        tempP = mystrcat(tempP, temp);
        tempP = mystrcat(tempP, "°C, ");
        tempP = mystrcat(tempP, weatherCond);
        *(tempP) = '\0'; // Make sure there is a null char at the end


        // Add timeoffset to sunrise & sunset and concat to sunRiseSetStr
        sunrise += timeoffset;
        sunset  += timeoffset;

        memset(sunRiseSetStr, 0, sizeof(sunRiseSetStr)); // Clear previous content

        char *sunP = sunRiseSetStr; // Pointer to sunRiseSetStr for mystrcat
        char buf[3] = "";           // Temp arr for itoa()

        sunP = mystrcat(sunP, "x");                     // Placeholder for custom arrow up char, will be displayed in update()
        sunP = mystrcat(sunP, Config::miniClockFormat); // Concat miniClockFormat to replace hour and minutes in a sec
        strrpl(sunRiseSetStr, "hh", lcd.toFixedLengthNumber(buf, hour(sunrise), 2), sizeof(sunRiseSetStr) - 1);
        strrpl(sunRiseSetStr, "mm", lcd.toFixedLengthNumber(buf, minute(sunrise), 2), sizeof(sunRiseSetStr) - 1);
        sunP = mystrcat(sunP, " x");                    // We can keep using our pointer as the length didn't change because of lcd.toFixedLengthNumber (and placeholder for arrow again)
        sunP = mystrcat(sunP, Config::miniClockFormat); // Concat miniClockFormat again for sunset
        strrpl(sunRiseSetStr, "hh", lcd.toFixedLengthNumber(buf, hour(sunset), 2), sizeof(sunRiseSetStr) - 1);
        strrpl(sunRiseSetStr, "mm", lcd.toFixedLengthNumber(buf, minute(sunset), 2), sizeof(sunRiseSetStr) - 1);
        *(sunP) = '\0';                                 // Make sure there is a null char at the end

        // Refresh timestamp so the next update will be in updateIntervalWeather ms 
        lastWeatherRefresh = millis();
        
        // Clear "Weather" and "Loading..."
        lcd.setCursor(0, 0);
        lcd.print("       ");
        lcd.clearLine(2);

        debug(F("weather page: Refresh done"));
    }
}