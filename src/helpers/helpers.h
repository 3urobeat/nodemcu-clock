/*
 * File: helpers.h
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 29.11.2021 17:37:59
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

void clearLine(LiquidCrystal_PCF8574 lcd, int maxcol, int row);
void centerPrint(String str, LiquidCrystal_PCF8574 lcd, int maxcol, int row, bool callclearLine);
void getLocation(LiquidCrystal_PCF8574 lcd, String openweathermaptoken, String *lat, String *lon, String *city, int *timeoffset);
ESP8266WiFiClass initWifi(LiquidCrystal_PCF8574 lcd, String wifiSSID[], String wifiPW[], size_t ssidamount, int maxcol, int row);
DynamicJsonDocument httpGetJson(String url);
DynamicJsonDocument httpsGetJson(String url);
String getDate(NTPClient timeClient, int timeoffset, String dateformat);
String getTime(NTPClient timeClient, int timeoffset, String timeformat);