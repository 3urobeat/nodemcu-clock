/*
 * File: weather.cpp
 * Project: pages
 * Created Date: 05.09.2021 17:53:00
 * Author: 3urobeat
 * 
 * Last Modified: 28.11.2021 16:29:20
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

String temp;        //store the two interesting weather values
String description;


void weatherpage(LiquidCrystal_PCF8574 lcd, String openweathermaptoken, String lat, String lon, String city, int maxcol)
{
    if (lastWeatherRefresh == 0 || lastWeatherRefresh + updateinterval <= millis())
    {
        DynamicJsonDocument result = httpGetJson("http://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + openweathermaptoken);

        //refresh both weather values
        temp = (String) round((double) result["main"]["temp"] - 273.15);
        description = (String) result["weather"][0]["main"];

        temp.replace(".00", ""); //You are probably thinking: Wtf? - and I completely agree with you. The output from round() contains decimals (18.00 for example) and it seems like removing those is challenging (spent ~15 min searching). And since this is already a String I'm just gonna do it like this. Fuck it.

        lastWeatherRefresh = millis();
    }

    centerPrint(city, lcd, maxcol, 1, false);
    centerPrint(temp + "°C, " + description, lcd, maxcol, 2, false); // (char)223 prints the degree symbol, putting it in a string like normal results in gibberish: https://forum.arduino.cc/t/print-degree-symbol-on-lcd/19073
}