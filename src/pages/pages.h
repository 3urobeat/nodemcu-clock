#include <LiquidCrystal_I2C.h>
#include <NTPClient.h>
#include <iostream>

void clockpage(LiquidCrystal_I2C lcd, NTPClient timeClient, int timeoffset, String dateformat, String timeformat, int maxcol);
void weatherpage(LiquidCrystal_I2C lcd, String openweathermaptoken, String lat, String lon, String city, int maxcol);
