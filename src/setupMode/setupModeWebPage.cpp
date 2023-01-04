/*
 * File: setupWebPage.cpp
 * Project: nodemcu-clock
 * Created Date: 24.12.2022 19:02:04
 * Author: 3urobeat
 * 
 * Last Modified: 04.01.2023 21:52:51
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

    if (var == "maxcol_input") return String(Config::maxcol);
    if (var == "pageOrder_input") return String(Config::pageOrder[0]) + ", " + String(Config::pageOrder[1]) + ", " + String(Config::pageOrder[2]);
    if (var == "showuntil_input") return String(Config::showuntil[0]) + ", " + String(Config::showuntil[1]) + ", " + String(Config::showuntil[2]);
    if (var == "alwaysShowTime_input") return Config::alwaysShowTime ? "checked" : ""; // Return checked or replace with nothing
    if (var == "clockWeekdaySwitch_input") return String(Config::clockWeekdaySwitch);

    return String();
}


// Serves the page when user visits webserver
void setupModeWebPage(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", webpage, processor); // webpage is defined in header file
}


// Saves webpage values to Config namespace when user clicks Save button
void setupModeWebPageSave(AsyncWebServerRequest *request) // TODO: Filter + from params
{
    // Update all config values (we technically don't need any hasArg() checks but let's keep them for good measure)
    if (request->hasArg("wifiSSID_input1")) request->arg("wifiSSID_input1").toCharArray(Config::wifiSSID[0], sizeof(Config::wifiSSID[0]));
    if (request->hasArg("wifiPW_input1")) request->arg("wifiPW_input1").toCharArray(Config::wifiPW[0], sizeof(Config::wifiPW[0]));
    if (request->hasArg("setupWifiPW_input")) request->arg("setupWifiPW_input").toCharArray(Config::setupWifiPW, sizeof(Config::setupWifiPW));

    if (request->hasArg("openweathermaptoken_input")) request->arg("openweathermaptoken_input").toCharArray(Config::openweathermaptoken, sizeof(Config::openweathermaptoken));
    if (request->hasArg("newsapitoken_input")) request->arg("newsapitoken_input").toCharArray(Config::newsapitoken, sizeof(Config::newsapitoken));

    if (request->hasArg("lat_input")) request->arg("lat_input").toCharArray(Config::lat, sizeof(Config::lat));
    if (request->hasArg("lon_input")) request->arg("lon_input").toCharArray(Config::lon, sizeof(Config::lon));
    if (request->hasArg("dateformat_input")) request->arg("dateformat_input").toCharArray(Config::dateformat, sizeof(Config::dateformat));
    if (request->hasArg("timeformat_input")) request->arg("timeformat_input").toCharArray(Config::timeformat, sizeof(Config::timeformat));
    if (request->hasArg("miniClockFormat_input")) request->arg("miniClockFormat_input").toCharArray(Config::miniClockFormat, sizeof(Config::miniClockFormat));

    if (request->hasArg("maxcol_input")) Config::maxcol = (uint8_t) request->arg("maxcol_input").toInt();
    //if (request->hasArg("pageOrder_input")) Config::pageOrder[0] = request->arg("pageOrder_input").toCharArray(); // TODO
    //if (request->hasArg("showuntil_input")) Config::showuntil[0] = request->arg("showuntil_input").toCharArray(); // TODO

    // If alwaysShowTime is unchecked it isn't included in the body so we must not check for hasArg()
    if (request->arg("alwaysShowTime_input") == "on") Config::alwaysShowTime = true;
        else Config::alwaysShowTime = false;

    if (request->hasArg("clockWeekdaySwitch_input")) Config::clockWeekdaySwitch = (uint16_t) request->arg("clockWeekdaySwitch_input").toInt();


    // Write config changes to fs
    writeConfigToStorage();
}