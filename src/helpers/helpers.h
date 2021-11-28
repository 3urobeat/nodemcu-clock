#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <NoiascaHW/lcd_i2c.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>

void clearLine(LiquidCrystal_PCF8574 lcd, int maxcol, int row);
void centerPrint(String str, LiquidCrystal_PCF8574 lcd, int maxcol, int row, bool callclearLine);
void getLocation(LiquidCrystal_PCF8574 lcd, String openweathermaptoken, String *lat, String *lon, String *city, int *timeoffset);
ESP8266WiFiClass initWifi(LiquidCrystal_PCF8574 lcd, String wifiSSID, String wifiPW, int maxcol, int row);
DynamicJsonDocument httpGetJson(String url);
DynamicJsonDocument httpsGetJson(String url);
String getDate(NTPClient timeClient, int timeoffset, String dateformat);
String getTime(NTPClient timeClient, int timeoffset, String timeformat);