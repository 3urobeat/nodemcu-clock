/*
 * File: setupMode.cpp
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:50:55
 * Author: 3urobeat
 * 
 * Last Modified: 25.12.2022 15:16:47
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

ESP8266WebServer webserver(80); // Init webserver on port 80


// Initialize setup switch input and check state
bool setupModeEnabledCheck()
{
    // Initialize switch pin as input
    pinMode(switchPin, INPUT_PULLUP);

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
    webserver.on("/", setupModeWebPage);
    webserver.begin();

    // Update screen
    lcd.clear();
    lcd.centerPrint("--- Setup Mode ---", 0);
    lcd.centerPrint("Connect to Wifi", 1);
    lcd.centerPrint("Visit 192.168.1.1", 2);
}


uint8_t animFrame = 0; // Tracking var for animation frame

// Displays animation on screen while setupMode is active
void setupModeLoop()
{
    // Display an animation so the device does not look like being softlocked
    lcd.setCursor(0, 3);
    lcd.animationPrint(lcd.animations.bouncearrow, 10, &animFrame, 8, 3);

    // Do webserver stuff
    webserver.handleClient();
}