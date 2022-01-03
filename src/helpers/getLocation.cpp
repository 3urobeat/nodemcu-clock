/*
 * File: getLocation.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 14:16:00
 * Author: 3urobeat
 * 
 * Last Modified: 30.12.2021 22:24:01
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <string.h>
#include "helpers.h"

void getLocation(LiquidCrystal_PCF8574 lcd, const char *openweathermaptoken, char *lat, char *lon, char *city, char *country, int *timeoffset)
{
    DynamicJsonDocument locationResult(256);

    //If the user didn't provide a lat & lon value then get values from geocoding api
    if (strlen(lat) == 0 && strlen(lon) == 0) {
        StaticJsonDocument<0> filter;
        filter.set(true);

        httpGetJson("http://ip-api.com/json?fields=lat,lon,offset,city,countryCode", &locationResult, filter);

        char temp[8];

        dtostrf(locationResult["lat"], 6, 4, temp); //convert double to string
        strcpy(lat, temp);

        dtostrf(locationResult["lon"], 6, 4, temp);
        strcpy(lon, temp);

        strcpy(city, locationResult["city"]);
        strcpy(country, locationResult["countryCode"]);

        *timeoffset = locationResult["offset"];

    } else { //...otherwise ping openweathermap once with the coords to get the city name and timeoffset
        StaticJsonDocument<128> filter;
        filter["name"] = true;
        filter["sys"]["country"] = true;
        filter["timezone"] = true;

        char fullstr[200] = "http://api.openweathermap.org/data/2.5/weather?lat=";
        char *p = fullstr;

        p = mystrcat(p, lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, openweathermaptoken);
        *(p) = '\0'; //add null char to the end

        httpGetJson(fullstr, &locationResult, filter);

        strcpy(city, locationResult["name"]);
        strcpy(country, locationResult["sys"]["country"]);

        *timeoffset = locationResult["timezone"];
    }
    
}