/*
 * File: main.h
 * Project: nodemcu-clock
 * Created Date: 30.11.2021 17:25:23
 * Author: 3urobeat
 * 
 * Last Modified: 19.01.2023 19:53:06
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
#include <NoiascaLiquidCrystal.h> // Article (download at the bottom): https://werner.rothschopf.net/202009_arduino_liquid_crystal_intro.htm
#include <NoiascaHW/lcd_PCF8574.h>
#include <lcdHelper.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>

#include <TimeLib.h>
#include <NTPClient.h>

#include <Preferences.h>

#include <ArduinoStreamParser.h>
#include <JsonHandler.h>


// Include config
#include "../config/config.h"

// Define a few kinda hardcoded values
#define ssidAmount 5


extern const char version[];
extern byte *stack_start;


// Setup
extern ESP8266WiFiClass WiFi;
extern uint16_t timeoffset;
extern char     city[64];
extern char     country[3];


// Loop
extern int8_t   currentPage;
extern uint32_t pageUpdate;
extern bool     hideMiniClock;


// Main
extern lcdHelper<LiquidCrystal_PCF8574> lcd;
extern WiFiUDP     ntpUDP;
extern NTPClient   timeClient;
extern Preferences prefs;


void setupHandler();
void loopHandler();
void nextPage();