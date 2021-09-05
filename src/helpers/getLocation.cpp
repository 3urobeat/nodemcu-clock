#include "helpers.h"

void getLocation(LiquidCrystal_I2C lcd, String *lat, String *lon, String *city, int *timeoffset)
{
    DynamicJsonDocument result = httpGetJson("http://ip-api.com/json?fields=lat,lon,offset,city");

    *lat = (String) result["lat"];
    *lon = (String) result["lon"];
    *city = (String) result["city"];
    *timeoffset = (int) result["offset"];
}