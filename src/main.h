/*
 * File: main.h
 * Project: nodemcu-clock
 * Created Date: 30.11.2021 17:25:23
 * Author: 3urobeat
 * 
 * Last Modified: 15.11.2022 13:23:28
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#pragma once

// Import all needed libs for the entire project
#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <NoiascaHW/lcd_PCF8574.h>
#include <lcdHelper.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>

#include <TimeLib.h>
#include <NTPClient.h>

#include <iostream>
#include <ArduinoJson.h>


namespace Config 
{
    extern const int maxcol;

    extern const char *wifiSSID[2];
    extern const char *wifiPW[2];

    extern char lat[8];
    extern char lon[8];

    extern const char openweathermaptoken[33];
    extern const char newsapitoken[33];

    extern const char dateformat[11];
    extern const char timeformat[9];
    extern const char miniClockFormat[6];

    extern const char *pageOrder[3];
    extern const int   showuntil[3];
    extern const bool  alwaysShowTime;
    extern const int   clockWeekdaySwitch;

    extern const bool DEBUG;
}

extern const char version[];


// Setup
extern char city[64];
extern char country[3];
extern int timeoffset;


// Loop
extern int currentPage;


// Main
extern lcdHelper<LiquidCrystal_PCF8574> lcd;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;


void setupHandler();
void loopHandler();