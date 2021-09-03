#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>

void clearLine(LiquidCrystal_I2C lcd, int maxcol, int row);
void centerPrint(String str, LiquidCrystal_I2C lcd, int maxcol, int row, bool callclearLine);
ESP8266WiFiClass initWifi(LiquidCrystal_I2C lcd, String wifiSSID, String wifiPW, int maxcol, int row);
DynamicJsonDocument httpsGetJson(String url);
String getDate(NTPClient timeClient, String dateformat);
String getTime(NTPClient timeClient, String timeformat);