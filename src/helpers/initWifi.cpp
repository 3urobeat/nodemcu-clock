/*
 * File: initWifi.cpp
 * Project: nodemcu-clock
 * Created Date: 2021-08-30 15:42:00
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-12 10:32:30
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


void(* resetFunc) (void) = 0; // Create a standard reset function


/**
 * Attempts to establish WiFi connection by checking all wifi networks set in config.
 * If no network provided in config could be found then the device will reset after 10 seconds.
 * @param row LCD display row to print connection related messages at (counted from 0)
 */
void initWifi(uint8_t row)
{
    lcd.clearLine(row); //clear line just to make sure no old characters are left

    lcd.setCursor(3, row);
    lcd.print("Searching WiFi");
    delay(500);

    // Search for wifi networks in range
    const uint32_t startedScanningTimestamp = millis();

    uint8_t n = WiFiLib.scanNetworks();
    bool found = false;

    char thisSSID[64]; // wifi SSIDs should have a max length of 32 characters

    // Try to connect by iterating for every found network over all networks in array (I tried doing this with indexOf() to avoid using a nested for loop but it had different results depending on the order in wifiSSID which was weird)
    lcd.setCursor(3, row);
    lcd.print("Connecting");

    for (uint8_t i = 0; i <= n; i++) {
        for (uint8_t j = 0; j < ssidAmount; j++) {
            WiFiLib.SSID(i).toCharArray(thisSSID, 64, 0);

            #ifdef CLOCK_DEBUG
                Serial.print("initWifi(): Found wifi SSID: ");
                Serial.println(thisSSID);
            #endif

            if (strlen(thisSSID) > 0 && strcmp(thisSSID, Config::wifiSSID[j]) == 0) { // Checking if thisSSID is >0 seems to make this check a bit more reliable, not exactly sure why though
                WiFiLib.begin(Config::wifiSSID[j], Config::wifiPW[j]);

                found = true; // Stop parent loop
                break;        // Stop this loop
            }
        }

        if (found) break;

        // Display error message if no network from wifiSSID array was found
        if (i == n && !found) {
            lcd.centerPrint("No match found!", row, false);
            delay(10000); // Wait a few sec and reset to rescan
            resetFunc();
        }
    }

    // Set hostname
    WiFiLib.hostname("nodemcu-clock");

    // Block futher execution until connection is established and show message with waiting animation
    uint8_t animationFrame = 1; // Skip first frame which is empty anyway

    while (WiFiLib.status() != WL_CONNECTED)
    {
        // Timeout wifi connect attempt after 60 seconds and reset device
        if (millis() - startedScanningTimestamp > 60000) {
            lcd.centerPrint("Connection Timeout", row);
            delay(10000);
            resetFunc();
        }

        lcd.animationPrint(lcd.animations.waiting, 5, &animationFrame, 13, row); // Use lcdHelper's animationPrint and default waiting animation

        handleBacklight(); // Update backlight to make switching on/off during connection phase possible

        delay(500);
    }

    // Connection established
    lcd.centerPrint("Connected!", row, true);
}
