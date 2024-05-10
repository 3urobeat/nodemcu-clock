/*
 * File: weather.cpp
 * Project: nodemcu-clock
 * Created Date: 2021-09-05 17:53:00
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-10 15:13:51
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "pages.h"


const uint32_t updateIntervalWeather = 1200000; // 20 minutes in ms
uint32_t       lastWeatherRefresh;

char           tempCondStr[20];   // Stores temp and weather condition
char           sunRiseSetStr[20]; // Stores sunrise and sunset time

uint32_t lastPageModWeather = millis();
bool     currentModWeather  = false;    // Tracks pageElemSwitch, as we only have 2 mods here (sunrise & sunset) we can use a bool


namespace weatherPage
{
    // Declare function here and define it later below to reduce clutter while being accessible from setup()
    bool refreshCache();


    /**
     * Setup the weather page
     */
    void setup()
    {

    }


    /**
     * Shows the weather page
     */
    void update()
    {
        // Check if user didn't provide an API key and display warning until page switches
        if (strlen(Config::weatherapitoken) == 0) {
            lcd.setCursor(0, 0);
            lcd.print("Weather");
            lcd.centerPrint("Error!", 1);
            lcd.centerPrint("No API key provided.", 3);
            return;
        }


        // Check if it's time to update weather cache (do this here instead of in setup so long showuntils don't fail)
        if (lastWeatherRefresh == 0 || lastWeatherRefresh + updateIntervalWeather <= millis()) {
            bool refreshSuccess = refreshCache();

            if (!refreshSuccess) { // Check if the request succeeded
                nextPage();
                return;
            }
        }


        // Print city name after refreshing to avoid it glitching into "Loading..."
        lcd.centerPrint(city, 1, false);

        // Switch between temp & cond and sunrise & sunset when pageElemSwitch ms passed since last mod switch
        if (Config::pageElemSwitch > 0 && millis() >= lastPageModWeather + Config::pageElemSwitch) {
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
    bool refreshCache()
    {
        debug(F("weather page: Refreshing cache"));

        // Display page name and loading message so the device doesn't look like it crashed
        lcd.setCursor(0, 0);
        lcd.print("Weather");
        lcd.centerPrint("Loading...", 2, false);


        // Construct URL
        char path[128] = "/data/2.5/weather?lat=";
        char *p = path;

        p = mystrcat(p, Config::lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, Config::lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, Config::weatherapitoken);
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

        httpGetJson("api.openweathermap.org", path, 80, parser);

        // Clear up memory
        delete(parser);


        // Check if weatherCond is empty, indicating that the request probably failed
        if (strlen(weatherCond) == 0) {
            lcd.centerPrint("Failed to refresh!", 2);
            delay(2500);
            return false;
        }


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
        return true;
    }
}
