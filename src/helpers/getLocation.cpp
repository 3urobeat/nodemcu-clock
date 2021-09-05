#include "helpers.h"

void getLocation(LiquidCrystal_I2C lcd, String *lat, String *lon, int *timeoffset)
{
    DynamicJsonDocument result = httpGetJson("http://ip-api.com/json?fields=lat,lon,offset");

    *lat = (String) result["lat"];
    *lon = (String) result["lon"];
    *timeoffset = (int) result["offset"];
}