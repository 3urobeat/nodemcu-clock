/*
 * File: initWifi.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 15:42:00
 * Author: 3urobeat
 * 
 * Last Modified: 30.11.2021 21:02:13
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


ESP8266WiFiClass initWifi(LiquidCrystal_PCF8574 lcd, String wifiSSID[], String wifiPW[], size_t ssidamount, int maxcol, int row) 
{
    clearLine(maxcol, row); //clear line just to make sure no old characters are left

    lcd.setCursor(3, row);
    lcd.print("Connecting");
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
    }

    //Block futher execution until connection is established and show message with waiting animation
    int dots = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        dots++; //add another dot
        if (dots > 3) dots = 0; //reset when 3 dots are present

        lcd.setCursor(13, row);
        lcd.print(std::string(dots, '.').c_str()); //add dots amount of dots behind "Connecting"
        lcd.print("    "); //Clear any dots that might be behind

        delay(500);
    }


    //Connection established
    centerPrint("Connected!", row, true);
    return WiFi;
}