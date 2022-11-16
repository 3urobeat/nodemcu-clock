/*
 * File: initWifi.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 15:42:00
 * Author: 3urobeat
 * 
 * Last Modified: 16.11.2022 19:15:00
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


ESP8266WiFiClass initWifi(size_t ssidamount, uint8_t row) 
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
        for (uint8_t j = 0; j < ssidamount; j++) {
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
    uint8_t dots = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        dots++; //add another dot
        if (dots > 3) dots = 0; //reset when 3 dots are present

        lcd.setCursor(13, row);
        lcd.print("    "); //Clear all dots

        for (uint8_t i = 0; i < dots; i++) {
            lcd.setCursor(13 + i, row); 
            lcd.print("."); //add dots amount of dots behind "Connecting"
        }

        delay(500);
    }


    //Connection established
    lcd.centerPrint("Connected!", row, true);
    return WiFi;
}