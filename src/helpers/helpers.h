/*
 * File: helpers.h
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 09.02.2022 14:08:53
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <NoiascaHW/lcd_PCF8574.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include "main.h"

void clearLine(int maxcol, int row);
void lcdSetCursor(int col, int row);
void lcdPrint(const char *str);
void centerPrint(const char *str, int row, bool callclearLine);
void movingPrint(const char *str, int row, bool callclearLine);
void getLocation(LiquidCrystal_PCF8574 lcd, const char *openweathermaptoken, char *lat, char *lon, char *city, char *country, int *timeoffset);
ESP8266WiFiClass initWifi(LiquidCrystal_PCF8574 lcd, const char *wifiSSID[], const char *wifiPW[], size_t ssidamount, int maxcol, int row);
void httpGetJson(const char *url, DynamicJsonDocument *doc, StaticJsonDocument<128> filter);
void httpsGetJson(const char *url, DynamicJsonDocument *doc, StaticJsonDocument<128> filter);
char *formatInt(char *buf, int value);
void getDate(char *dest, NTPClient timeClient, int timeoffset, const char *dateformat);
void getTime(char *dest, NTPClient timeClient, int timeoffset, const char *timeformat);
void strrpl(char *src, const char *oldchars, const char *newchars);
char *mystrcat(char* dest, const char* src);
size_t utf8_strlen(const char *str);