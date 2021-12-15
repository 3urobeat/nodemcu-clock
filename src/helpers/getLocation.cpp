/*
 * File: getLocation.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 14:16:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.12.2021 15:20:56
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"

void getLocation(LiquidCrystal_PCF8574 lcd, String openweathermaptoken, String *lat, String *lon, String *city, String *country, int *timeoffset)
{
    DynamicJsonDocument locationResult(1024);

    //If the user didn't provide a lat & lon value then get values from geocoding api
    if (*lat == "" && *lon == "") {
        httpGetJson("http://ip-api.com/json?fields=lat,lon,offset,city,country", &locationResult);

        //Only overwrite lat and lon if user didn't provide a location manually
        *lat = (String) locationResult["lat"];
        *lon = (String) locationResult["lon"];

        *city = (String) locationResult["city"];
        *country = (String) locationResult["countryCode"];
        *timeoffset = (int) locationResult["offset"];

    } else { //...otherwise ping openweathermap once with the coords to get the city name and timeoffset
        httpGetJson("http://api.openweathermap.org/data/2.5/weather?lat=" + *lat + "&lon=" + *lon + "&appid=" + openweathermaptoken, &locationResult);

        *city = (String) locationResult["name"];
        *country = (String) locationResult["sys"]["country"];
        *timeoffset = (int) locationResult["timezone"];
    }
    
}