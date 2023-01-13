/*
 * File: setupMode.cpp
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:50:55
 * Author: 3urobeat
 * 
 * Last Modified: 13.01.2023 18:25:24
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "setupMode.h"


const uint8_t switchPin = D0;
const IPAddress localIP(192, 168, 1, 1);
const IPAddress gatewayIP(192, 168, 1, 1);
const IPAddress subnet(255, 255, 255, 0);
const char setupWifiSSID[25] = "nodemcu-clock setup wifi";

bool setupModeEnabled = false;

AsyncWebServer webserver(80); // Init webserver on port 80


/**
 * Initialize setup switch input and check state
 * @param forceSetupMode (Optional, default: false) Force enables setupMode, use for example when configDetectFirstStart() returned true
 */
bool setupModeEnabledCheck(bool forceSetupMode)
{
    // Initialize switch pin as input
    pinMode(switchPin, INPUT_PULLUP);

    // Abort if forceSetupMode is true
    if (forceSetupMode) { // TODO: Display first-time welcome message?
        setupModeEnabled = true;
        return true;
    }

    // Read switch state and update setupModeEnabled
    setupModeEnabled = (digitalRead(switchPin) == LOW); // Set to true if return is LOW, aka switchPin is connected to GND, default is HIGH because of PULLUP

    return setupModeEnabled;
}


// Host wifi network and webserver for setupMode, is called from setup
void setupModeSetup()
{
    lcd.centerPrint("Entering Setup...", 3);
    delay(250);

    // Launch Wifi AP
    WiFi.softAPConfig(localIP, gatewayIP, subnet);
    WiFi.softAP(setupWifiSSID, Config::setupWifiPW);

    // Start webserver
    webserver.on("/", HTTP_GET, setupModeWebPage); // Call setupModeWebPage when user accesses root page, it handles displaying the webpage
    webserver.on("/post", HTTP_POST, setupModeWebPageSave);
    webserver.begin();

    // Update screen
    lcd.clear();
    lcd.centerPrint("--- Setup Mode ---", 0);
    lcd.centerPrint("Connect to Wifi", 1);
    lcd.centerPrint("Visit 192.168.1.1", 2);
}


uint32_t savedTime = 0; // Gets set by setupModeWebPageSave() to a timestamp if "Saved!" should be displayed for 5 sec instead of animation
uint8_t  animFrame = 0; // Tracking var for animation frame

// Displays animation on screen while setupMode is active
void setupModeLoop()
{
    if (millis() < savedTime + 5000 && savedTime != 0) {
        lcd.centerPrint("- Settings Saved -", 3);
    } else {
        // Clear line if we just switched back to animation
        if (millis() - savedTime < 5300) lcd.clearLine(3);

        // Display an animation so the device does not look like being softlocked
        lcd.setCursor(0, 3);
        lcd.animationPrint(lcd.animations.bouncearrow, 10, &animFrame, 8, 3);
    }

    debugMemory();
}