/*
 * File: helpers.h
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 22:36:23
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#pragma once

#include "main.h"
#include "helpersJsonParser.h"


bool configDetectFirstStart();
void readConfigFromStorage();
void writeConfigToStorage();

// Make debugMemory() available when debug mode is enabled, otherwise alias debugMemory() to nothing
#ifdef CLOCK_DEBUG
    void debugMemory(const __FlashStringHelper *str = nullptr); // Optional str param to log additional msg
#else
    #define debugMemory(x) 
#endif

void getLocation();

void httpGetJson(const char *url, JsonHandler* handler);

ESP8266WiFiClass initWifi(uint8_t row);

void miniClock(bool hideMiniClock);

void strrpl(char *src, const char *oldchars, const char *newchars, uint16_t maxlen);
char *mystrcat(char* dest, const char* src);

void getDate(char *dest, uint16_t destLen, NTPClient timeClient, uint16_t timeoffset);
void getTime(char *dest, uint16_t destLen, NTPClient timeClient, uint16_t timeoffset, const char *format);