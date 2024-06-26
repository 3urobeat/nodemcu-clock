/*
 * File: helpers.h
 * Project: nodemcu-clock
 * Created Date: 2021-08-30 11:19:00
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-12 10:14:00
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "main.h"
#include "helpersJsonParser.h"


void handleBacklight();
void setupBacklight();

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

void initWifi(uint8_t row);

void strrpl(char *src, const char *oldchars, const char *newchars, uint16_t maxlen);
char *mystrcat(char* dest, const char* src);

void getDate(char *dest, uint16_t destLen);
void getTime(char *dest, uint16_t destLen, const char *format);

extern bool miniClockActive;
extern bool loadingActive;
void updateTitleBar(const char *title, bool hideMiniClock);
void indicateLoading(bool clearIcon = false);
void miniClock();
