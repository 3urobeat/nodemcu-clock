/*
 * File: setup.cpp
 * Project: nodemcu-clock
 * Created Date: 2022-10-30 19:01:26
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-12 10:13:49
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers/helpers.h"
#include "setupMode/setupMode.h"


#ifdef ESP8266
ESP8266WiFiClass WiFiLib;
#elif ESP32
WiFiClass WiFiLib;
#endif

uint16_t timeoffset; // Timezone offset in seconds

char city[64];   // Yes, we can store the longest city names: https://largest.org/geography/city-names/
char country[3]; // Two char long country codes


/**
 * Called on setup, initializes stuff
 */
void setupHandler()
{
    // Print some more information if debug mode is enabled
    #ifdef CLOCK_DEBUG
        delay(2500); // Delay so that the PlatformIO Serial console doesn't go stupid as it starts slower than the Arduino boots
        Serial.begin(9600);
        Serial.setDebugOutput(true);
        Serial.println(F("\n\nDebug Mode enabled! Starting..."));
        debug(); // Log free memory once on start
    #endif

    // Initiate display & preferences lib
    Wire.begin();
    lcd.begin();
    setupBacklight();
    prefs.begin("nodemcu-clock");

    // Register custom lcd chars
    createCustomChars();

    // Print startup screen
    lcd.centerPrint("nodemcu-clock", 0, true);
    lcd.centerPrint(version, 1, true);
    delay(1000);
    lcd.centerPrint("Loading...", 3, true);

    debug(F("Finished setting up display"));

    // Load config values from storage
    bool isFirstStart = configDetectFirstStart();

    #ifndef CLOCK_IGNOREFS // Skip reading settings if IGNOREFS is enabled
        if (!isFirstStart) readConfigFromStorage(); // Read config in fs if not first start
    #endif

    // Check for setup mode and continue with desired mode (or force setupMode and display welcome msg when config is empty)
    if (setupModeEnabledCheck(isFirstStart)) {

        // Host wifi network and webserver
        setupModeSetup();

    } else {

        // Connect to wifi
        initWifi(3);
        delay(500);
        lcd.centerPrint("Loading...", 3, true);

        // Geolocate the used IP to get coordinates and the timeoffset
        getLocation();
        debug(F("getLocation() helper finished"));

        // Start syncing time
        timeClient.begin();

        // Clear display and let page manager do page manager things (aka show a page lol)
        lcd.clear();
        debug(F("\n----\nSetup done, entering loop!"));
    }
}
