/*
 * File: weather.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 17:53:00
 * Author: 3urobeat
 * 
 * Last Modified: 29.12.2021 12:50:52
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <ArduinoJson.h>

#include "pages.h"
#include "helpers/helpers.h"

int updateinterval = 600000; //10 minutes in ms

unsigned long lastWeatherRefresh;

char fullStr[30]; //store string to show


void weatherpage(const char *openweathermaptoken, const char *lat, const char *lon, const char *city)
{
    if (lastWeatherRefresh == 0 || lastWeatherRefresh + updateinterval <= millis())
    {
        //create filter
        StaticJsonDocument<128> filter;
        
        filter["main"]["temp"] = true;
        filter["weather"][0]["main"] = true;
        
        //Construct URL
        char requestStr[200] = "http://api.openweathermap.org/data/2.5/weather?lat=";
        char *p = requestStr;

        p = mystrcat(p, lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, openweathermaptoken);
        *(p) = '\0'; //add null char to the end
        
        //Make API call
        DynamicJsonDocument weatherResult(256);
        httpGetJson(requestStr, &weatherResult, filter);


        //refresh fullStr
        itoa((int) round((double) weatherResult["main"]["temp"] - 273.15), fullStr, 10); //convert temp to string and write into fullStr

        char *fSp = fullStr;
        
        fSp = mystrcat(fSp, "°C, ");
        fSp = mystrcat(fSp, weatherResult["weather"][0]["main"]);
        *(p) = '\0'; //add null char to the end just to be sure, I think there might already be one

        lastWeatherRefresh = millis();
    }    

    centerPrint(city, 1, false);
    centerPrint(fullStr, 2, false); // (char)223 prints the degree symbol, putting it in a string like normal results in gibberish: https://forum.arduino.cc/t/print-degree-symbol-on-lcd/19073
}