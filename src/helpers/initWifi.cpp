/*
 * File: initWifi.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 15:42:00
 * Author: 3urobeat
 * 
 * Last Modified: 26.10.2022 13:42:49
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <iostream>
#include "helpers.h"


void(* resetFunc) (void) = 0; // create a standard reset function


ESP8266WiFiClass initWifi(LiquidCrystal_PCF8574 lcd, const char *wifiSSID[], const char *wifiPW[], size_t ssidamount, int maxcol, int row) 
{
    lcd.clearLine(row); //clear line just to make sure no old characters are left

    lcd.setCursor(3, row);
    lcd.print("Connecting");
    delay(500);

    //Search for wifi networks in range
    int  n     = WiFi.scanNetworks();
    bool found = false;

    char thisSSID[64]; //wifi SSIDs should have a max length of 32 characters

    //Try to connect by iterating for every found network over all networks in array (I tried doing this with indexOf() to avoid using a nested for loop but it had different results depending on the order in wifiSSID which was weird)
    for (int i = 0; i <= n; i++) {
        for (unsigned int j = 0; j < ssidamount; j++) {
            WiFi.SSID(i).toCharArray(thisSSID, 64, 0);

            if (strcmp(thisSSID, wifiSSID[j]) == 0) {

                WiFi.begin(wifiSSID[j], wifiPW[j]);

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

    //Block futher execution until connection is established and show message with waiting animation
    int dots = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        dots++; //add another dot
        if (dots > 3) dots = 0; //reset when 3 dots are present

        lcd.setCursor(13, row);
        lcd.print("    "); //Clear all dots

        for (int i = 0; i < dots; i++) {
            lcd.setCursor(13 + i, row); 
            lcd.print("."); //add dots amount of dots behind "Connecting"
        }

        delay(500);
    }


    //Connection established
    lcd.centerPrint("Connected!", row, true);
    return WiFi;
}