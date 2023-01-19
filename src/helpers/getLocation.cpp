/*
 * File: getLocation.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 14:16:00
 * Author: 3urobeat
 * 
 * Last Modified: 19.01.2023 19:50:43
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


/**
 * Geolocates device using IP (if not set in config) and retreives city name & contry code
 */
void getLocation()
{
    // If the user didn't provide a lat & lon value in Config then get values from geocoding API
    if (strlen(Config::lat) == 0 && strlen(Config::lon) == 0) {
        
        // Create obj of our parser class and make request
        GetLocationJsonHandler1 *parser = new GetLocationJsonHandler1();

        httpGetJson("ip-api.com", "/json?fields=lat,lon,offset,city,countryCode", 80, parser);

        // Clear up memory
        delete(parser);

    } else { // ...otherwise ping openweathermap once with the coords to get the city name and timeoffset
    
        // Construct URL
        char path[128] = "/data/2.5/weather?lat=";
        char *p = path;

        p = mystrcat(p, Config::lat);
        p = mystrcat(p, "&lon=");
        p = mystrcat(p, Config::lon);
        p = mystrcat(p, "&appid=");
        p = mystrcat(p, Config::openweathermaptoken);
        *(p) = '\0'; // Make sure there is a null char at the end

        // Create obj of our parser class and make request
        GetLocationJsonHandler2 *parser = new GetLocationJsonHandler2();

        httpGetJson("api.openweathermap.org", path, 80, parser);

        // Clear up memory
        delete(parser);
    }
}