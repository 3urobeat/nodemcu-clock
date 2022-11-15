/*
 * File: getLocation.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 14:16:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.11.2022 13:18:06
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


void getLocation()
{
    DynamicJsonDocument locationResult(256);

    //If the user didn't provide a lat & lon value then get values from geocoding api
    if (strlen(Config::lat) == 0 && strlen(Config::lon) == 0) {
        StaticJsonDocument<0> filter;
        filter.set(true);

        httpGetJson("http://ip-api.com/json?fields=lat,lon,offset,city,countryCode", &locationResult, filter);

        char temp[8];

        dtostrf(locationResult["lat"], 6, 4, temp); //convert double to string
        strcpy(Config::lat, temp);

        dtostrf(locationResult["lon"], 6, 4, temp);
        strcpy(Config::lon, temp);

        strncpy(city, locationResult["city"], sizeof(city) - 1);
        strncpy(country, locationResult["countryCode"], sizeof(country) - 1);

        timeoffset = locationResult["offset"]; // Update timeoffset in main.h

    } else { //...otherwise ping openweathermap once with the coords to get the city name and timeoffset
    
        StaticJsonDocument<128> filter;
        filter["name"] = true;
        filter["sys"]["country"] = true;
        filter["timezone"] = true;

        char fullstr[200] = "http://api.openweathermap.org/data/2.5/weather?lat=";
        char *p = fullstr;

        p = mystrcat(p, Config::lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, Config::lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, Config::openweathermaptoken);
        *(p) = '\0'; //add null char to the end

        httpGetJson(fullstr, &locationResult, filter);

        // Write result into vars exported from setup.cpp
        strncpy(city, locationResult["name"], sizeof(city) - 1);
        strncpy(country, locationResult["sys"]["country"], sizeof(country) - 1);

        timeoffset = locationResult["timezone"]; // Update timeoffset in main.h
    }
    
}