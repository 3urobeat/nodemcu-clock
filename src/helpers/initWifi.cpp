/*
 * File: initWifi.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 15:42:00
 * Author: 3urobeat
 * 
 * Last Modified: 09.01.2023 16:44:55
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


void(* resetFunc) (void) = 0; // create a standard reset function


ESP8266WiFiClass initWifi(uint8_t row) 
{
    lcd.clearLine(row); //clear line just to make sure no old characters are left

    lcd.setCursor(3, row);
    lcd.print("Connecting");
    delay(500);

    //Search for wifi networks in range
    uint8_t n = WiFi.scanNetworks();
    bool found = false;

    char thisSSID[64]; //wifi SSIDs should have a max length of 32 characters

    //Try to connect by iterating for every found network over all networks in array (I tried doing this with indexOf() to avoid using a nested for loop but it had different results depending on the order in wifiSSID which was weird)
    for (uint8_t i = 0; i <= n; i++) {
        for (uint8_t j = 0; j < ssidAmount; j++) {
            WiFi.SSID(i).toCharArray(thisSSID, 64, 0);

            if (strcmp(thisSSID, Config::wifiSSID[j]) == 0) {

                WiFi.begin(Config::wifiSSID[j], Config::wifiPW[j]);

                found = true; //stop parent loop
                break; //stop this loop
            }
        }

        if (found) break;

        //Display error message if no network from wifiSSID array was found
        if (i == n && !found) {
            lcd.centerPrint("No match found!", row, false);
            delay(10000); //wait a few sec and reset to rescan
            resetFunc();
        }
    }

    // Block futher execution until connection is established and show message with waiting animation
    uint8_t animationFrame = 1; // Skip first frame which is empty anyway

    while (WiFi.status() != WL_CONNECTED)
    {
        lcd.animationPrint(lcd.animations.waiting, 5, &animationFrame, 13, row); // Use lcdHelper's animationPrint and default waiting animation

        delay(500);
    }

    //Connection established
    lcd.centerPrint("Connected!", row, true);

    return WiFi;
}