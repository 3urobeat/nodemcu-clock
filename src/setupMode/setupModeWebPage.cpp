/*
 * File: setupWebPage.cpp
 * Project: nodemcu-clock
 * Created Date: 24.12.2022 19:02:04
 * Author: 3urobeat
 * 
 * Last Modified: 30.12.2022 23:07:31
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "setupMode.h"


// Processor function to dynamically replace placeholder variables in webpage
String processor(const String& var) // I need to use String here because the library forces me to... https://github.com/me-no-dev/ESPAsyncWebServer#send-large-webpage-from-progmem-containing-templates-and-extra-headers
{
    if (var == "wifiSSID_input1") return String(Config::wifiSSID[0]);
    if (var == "wifiPW_input1") return String(Config::wifiPW[0]);
    if (var == "setupWifiPW_input") return String(Config::setupWifiPW);

    if (var == "openweathermaptoken_input") return String(Config::openweathermaptoken);
    if (var == "newsapitoken_input") return String(Config::newsapitoken);

    if (var == "lat_input") return String(Config::lat);
    if (var == "lon_input") return String(Config::lon);
    if (var == "dateformat_input") return String(Config::dateformat);
    if (var == "timeformat_input") return String(Config::timeformat);
    if (var == "miniClockFormat_input") return String(Config::miniClockFormat);

    if (var == "maxcol_input") return String(Config::maxcol); // TODO: Doesn't work for some reason
    if (var == "pageOrder_input") return String(Config::pageOrder[0]) + ", " + String(Config::pageOrder[1]) + ", " + String(Config::pageOrder[2]);
    if (var == "showuntil_input") return String(Config::showuntil[0]) + ", " + String(Config::showuntil[1]) + ", " + String(Config::showuntil[2]);
    if (var == "alwaysShowTime_input") return Config::alwaysShowTime ? "checked" : ""; // Return checked or replace with nothing // TODO: Doesn't work for some reason
    if (var == "clockWeekdaySwitch_input") return String(Config::clockWeekdaySwitch); // TODO: Doesn't work for some reason
    
    return String();
}


// Serves the page when user visits webserver
void setupModeWebPage(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", webpage, processor); // webpage is defined in header file
}