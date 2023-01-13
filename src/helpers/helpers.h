/*
 * File: helpers.h
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 13.01.2023 17:59:31
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

void getLocation();

void httpGetJson(const char *url, JsonHandler* handler);

ESP8266WiFiClass initWifi(uint8_t row);

void miniClock(bool hideMiniClock);

void strrpl(char *src, const char *oldchars, const char *newchars);
char *mystrcat(char* dest, const char* src);

void getDate(char *dest, NTPClient timeClient, uint16_t timeoffset);
void getTime(char *dest, NTPClient timeClient, uint16_t timeoffset, const char *format);