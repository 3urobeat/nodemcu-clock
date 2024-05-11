/*
 * File: main.h
 * Project: nodemcu-clock
 * Created Date: 2021-11-30 17:25:23
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-11 12:02:48
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

// Import all needed libs for the entire project
#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (download at the bottom): https://werner.rothschopf.net/202009_arduino_liquid_crystal_intro.htm
#include <NoiascaHW/lcd_PCF8574.h>
#include <lcdHelper.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#include <WiFiClientSecure.h>
#else
#error Platform not supported
#endif

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
#ifdef ESP8266
extern ESP8266WiFiClass WiFiLib;
#elif ESP32
extern WiFiClass WiFiLib;
#endif
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
void indicateLoading(bool clearIcon = false);
