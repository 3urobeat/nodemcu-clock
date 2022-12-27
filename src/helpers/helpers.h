/*
 * File: helpers.h
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 27.12.2022 14:40:23
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


bool configDetectFirstStart();
void readConfigFromStorage();
void writeConfigToStorage();

void getLocation();

void httpGetJson(const char *url, DynamicJsonDocument *doc, StaticJsonDocument<128> filter);
void httpsGetJson(const char *url, DynamicJsonDocument *doc, StaticJsonDocument<128> filter);

ESP8266WiFiClass initWifi(size_t ssidamount, uint8_t row);

void miniClock(bool hideMiniClock);

void strrpl(char *src, const char *oldchars, const char *newchars);
char *mystrcat(char* dest, const char* src);

void getDate(char *dest, NTPClient timeClient, uint16_t timeoffset);
void getTime(char *dest, NTPClient timeClient, uint16_t timeoffset, const char *format);