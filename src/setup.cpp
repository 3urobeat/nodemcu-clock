/*
 * File: setup.cpp
 * Project: nodemcu-clock
 * Created Date: 30.10.2022 19:01:26
 * Author: 3urobeat
 * 
 * Last Modified: 03.01.2023 13:55:37
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "helpers/helpers.h"
#include "setupMode/setupMode.h"


uint16_t timeoffset; // Timezone offset in seconds

char city[64];   // Yes, we can store the longest city names: https://largest.org/geography/city-names/
char country[3]; // Two char long country codes

const size_t ssidAmount = sizeof Config::wifiSSID / sizeof Config::wifiSSID[0]; // Calculate wifiSSID size to avoid -Wsizeof-array-argument warning


/**
 * Called on setup, initializes stuff
 */
void setupHandler()
{
    // Initiate display
    Wire.begin();
    lcd.begin();
    lcd.backlight();
    prefs.begin("nodemcu-clock");

    // Debug?
    if (Config::DEBUG) {
        Serial.begin(9600);
        Serial.println("Begin!");
    }

    // Print startup screen
    lcd.centerPrint("nodemcu-clock", 0, true);
    lcd.centerPrint(version, 1, true);
    delay(1000);
    lcd.centerPrint("Loading...", 3, true);

    // Load config values from storage
    bool isFirstStart = configDetectFirstStart();

    if (!isFirstStart) readConfigFromStorage(); // Read config in fs if not first start

    // Check for setup mode and continue with desired mode (or force setupMode and display welcome msg when config is empty)
    if (setupModeEnabledCheck(isFirstStart)) {

        // Host wifi network and webserver
        setupModeSetup();

    } else {

        // Connect to wifi
        initWifi(ssidAmount, 3);
        delay(500);
        lcd.centerPrint("Loading...", 3, true);

        // Geolocate the used IP to get coordinates and the timeoffset
        getLocation();

        // Start syncing time
        timeClient.begin();

        // Clear display and let page manager do page manager things (aka show a page lol)
        lcd.clear();
    }
}