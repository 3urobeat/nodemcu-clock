#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <NoiascaHW/lcd_i2c.h>
#include <NTPClient.h>
#include <iostream>

void clockpage(LiquidCrystal_PCF8574 lcd, NTPClient timeClient, int timeoffset, String dateformat, String timeformat, int maxcol);
void weatherpage(LiquidCrystal_PCF8574 lcd, String openweathermaptoken, String lat, String lon, String city, int maxcol);
