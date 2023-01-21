/*
 * File: helpers.h
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 21.01.2023 13:15:07
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

void createCustomChars();

// Make debug() available when debug mode is enabled, otherwise alias debug() to nothing
#ifdef CLOCK_DEBUG
    void debug(const __FlashStringHelper *str = nullptr); // Optional str param to log additional msg
#else
    #define debug(x) 
#endif

void getLocation();

int16_t httpGetJson(const char *host, const char *path, uint16_t port, JsonHandler* handler, const char *extraHeaders = nullptr, ArudinoStreamParser *parserLib = nullptr);

ESP8266WiFiClass initWifi(uint8_t row);

void miniClock(bool hideMiniClock);

void strrpl(char *src, const char *oldchars, const char *newchars, uint16_t maxlen);
char *mystrcat(char* dest, const char* src);

void getDate(char *dest, uint16_t destLen, NTPClient timeClient);
void getTime(char *dest, uint16_t destLen, NTPClient timeClient, const char *format);