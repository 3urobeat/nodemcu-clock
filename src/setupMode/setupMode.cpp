/*
 * File: setupMode.cpp
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:50:55
 * Author: 3urobeat
 * 
 * Last Modified: 30.06.2023 09:47:10
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/3urobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "setupMode.h"

bool setupModeEnabled = false;


/**
 * Initialize setup switch input and check state
 * @param forceSetupMode (Optional, default: false) Force enables setupMode, use for example when configDetectFirstStart() returned true
 */
bool setupModeEnabledCheck(bool forceSetupMode)
{   
    #ifdef ESP8266
        const uint8_t switchPin = D0;
    #elif ESP32
        const uint8_t switchPin = 19;
    #endif
    
    
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


/**
 * Host wifi network and webserver for setupMode, is called from setup
 */
void setupModeSetup()
{
    lcd.centerPrint("Entering Setup...", 3);
    delay(250);

    // Define IP and AP name (not at top scope so it doesn't get allocated when not in setupMode)
    const IPAddress localIP(192, 168, 1, 1);
    const IPAddress gatewayIP(192, 168, 1, 1);
    const IPAddress subnet(255, 255, 255, 0);
    
    const char setupWifiSSID[25] = "nodemcu-clock setup wifi";

    // Launch Wifi AP
    WiFiLib.softAPConfig(localIP, gatewayIP, subnet);
    WiFiLib.softAP(setupWifiSSID, Config::setupWifiPW);

    // Start webserver
    AsyncWebServer *webserver = new AsyncWebServer(80); // Init webserver on port 80 (no delete later as obj gets destroyed on reset)
    
    webserver->on("/", HTTP_GET, setupModeWebPage); // Call setupModeWebPage when user accesses root page, it handles displaying the webpage
    webserver->on("/post", HTTP_POST, setupModeWebPageSave);
    webserver->begin();

    // Update screen
    lcd.clear();
    lcd.centerPrint("--- Setup Mode ---", 0);
    lcd.centerPrint("Connect to Wifi", 1);
    lcd.centerPrint("Visit 192.168.1.1", 2);
}


uint32_t savedTime = 0; // Gets set by setupModeWebPageSave() to a timestamp if "Saved!" should be displayed for 5 sec instead of animation

/**
 * Displays animation on screen while setupMode is active
 */
void setupModeLoop()
{
    static uint8_t animFrame = 0; // Tracking var for animation frame

    // Check if we are supposed to display "Settings Saved", otherwise refresh animation
    if (millis() < savedTime + 5000 && savedTime != 0) {
        lcd.centerPrint("- Settings Saved -", 3);
    } else {
        // Clear line if we just switched back to animation
        if (millis() - savedTime < 5300) lcd.clearLine(3);

        // Display an animation so the device does not look like being softlocked
        lcd.animationPrint(lcd.animations.bouncearrow, 10, &animFrame, 8, 3);
    }

    debug();
}