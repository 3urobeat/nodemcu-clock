/*
 * File: initWifi.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 15:42:00
 * Author: 3urobeat
 * 
 * Last Modified: 01.12.2021 15:01:29
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


ESP8266WiFiClass initWifi(LiquidCrystal_PCF8574 lcd, String wifiSSID[], String wifiPW[], size_t ssidamount, int maxcol, int row) 
{
    clearLine(maxcol, row); //clear line just to make sure no old characters are left

    lcdSetCursor(3, row);
    lcdPrint("Connecting");
    delay(500);

    //Search for wifi networks in range
    int  n     = WiFi.scanNetworks();
    bool found = false;

    //Try to connect by iterating for every found network over all networks in array (I tried doing this with indexOf() to avoid using a nested for loop but it had different results depending on the order in wifiSSID which was weird)
    for (int i = 0; i <= n; i++) {
        for (unsigned int j = 0; j <= ssidamount; j++) {

            if (WiFi.SSID(i) == wifiSSID[j]) {
                WiFi.begin(wifiSSID[j], wifiPW[j]);

                found = true; //stop parent loop
                break; //stop this loop
            }
        }

        if (found) break;

        //Display error message if no network from wifiSSID array was found
        if (i == n && !found) {
            centerPrint("No match found!", row, false);
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

        lcdSetCursor(13, row);
        lcdPrint(std::string(dots, '.').c_str()); //add dots amount of dots behind "Connecting"
        lcdPrint("    "); //Clear any dots that might be behind

        delay(500);
    }


    //Connection established
    centerPrint("Connected!", row, true);
    return WiFi;
}