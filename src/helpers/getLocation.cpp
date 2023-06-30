/*
 * File: getLocation.cpp
 * Project: nodemcu-clock
 * Created Date: 05.09.2021 14:16:00
 * Author: 3urobeat
 * 
 * Last Modified: 30.06.2023 09:47:10
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/3urobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
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
        p = mystrcat(p, Config::weatherapitoken);
        *(p) = '\0'; // Make sure there is a null char at the end

        // Create obj of our parser class and make request
        GetLocationJsonHandler2 *parser = new GetLocationJsonHandler2();

        httpGetJson("api.openweathermap.org", path, 80, parser);

        // Clear up memory
        delete(parser);
    }
}