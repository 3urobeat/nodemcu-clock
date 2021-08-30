#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

void clearLine(LiquidCrystal_I2C lcd, int maxcol, int row);
void centerPrint(String str, LiquidCrystal_I2C lcd, int maxcol, int row);
ESP8266WiFiClass initWifi(LiquidCrystal_I2C lcd, String wifiSSID, String wifiPW, int maxcol, int row);