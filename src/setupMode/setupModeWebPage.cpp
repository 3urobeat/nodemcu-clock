/*
 * File: setupWebPage.cpp
 * Project: nodemcu-clock
 * Created Date: 24.12.2022 19:02:04
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 16:06:31
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
    // Fill "wifiNetworksIncluded" array with already stored wifi networks - this is prob quite inefficient but idc too much in setupMode
    char wifiSSIDTemp[10] = "wifiSSID0";
    char wifiPWTemp[8]    = "wifiPW0";
    
    for (uint8_t i = 1; i <= ssidAmount; i++) { // Input names start with 1, not with 0 in this case
        // Update both char arrs with index i for this iteration
        wifiSSIDTemp[8] = '0' + i;
        wifiPWTemp[6]   = '0' + i; // Quick trick to convert int to char by incrementing ascii code of char '0'

        if (var == wifiSSIDTemp) return String(Config::wifiSSID[i - 1]);
        if (var == wifiPWTemp)   return String(Config::wifiPW[i - 1]);
    }


    // Check for all other config values
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
void setupModeWebPageSave(AsyncWebServerRequest *request)
{
    // Update all wifi networks included in response
    char wifiSSID_inputTemp[16] = "wifiSSID_input0";
    char wifiPW_inputTemp[14]   = "wifiPW_input0";

    // Clear the whole array first to also remove any networks the user removed in the response
    memset(Config::wifiSSID, 0, sizeof(Config::wifiSSID));
    memset(Config::wifiPW, 0, sizeof(Config::wifiPW));

    for (uint8_t i = 1; i <= ssidAmount; i++) { // Input names start with 1, not with 0 in this case
        // Update both char arrs with index i for this iteration
        wifiSSID_inputTemp[14] = '0' + i;
        wifiPW_inputTemp[12]   = '0' + i; // Quick trick to convert int to char by incrementing ascii code of char '0'

        // Update element if included in response, it otherwise stays empty
        if (request->hasArg(wifiSSID_inputTemp) && request->hasArg(wifiPW_inputTemp)) {
            request->arg(wifiSSID_inputTemp).toCharArray(Config::wifiSSID[i - 1], sizeof(Config::wifiSSID[0])); // Subtract 1 from i to get back to 0 based indexing of Config
            request->arg(wifiPW_inputTemp).toCharArray(Config::wifiPW[i - 1], sizeof(Config::wifiPW[0]));
        }
    }


    // Update all other config values (we technically don't need any hasArg() checks but let's keep them for good measure)
    if (request->hasArg("setupWifiPW_input")) request->arg("setupWifiPW_input").toCharArray(Config::setupWifiPW, sizeof(Config::setupWifiPW));

    if (request->hasArg("openweathermaptoken_input")) request->arg("openweathermaptoken_input").toCharArray(Config::openweathermaptoken, sizeof(Config::openweathermaptoken));
    if (request->hasArg("newsapitoken_input")) request->arg("newsapitoken_input").toCharArray(Config::newsapitoken, sizeof(Config::newsapitoken));

    if (request->hasArg("lat_input")) request->arg("lat_input").toCharArray(Config::lat, sizeof(Config::lat));
    if (request->hasArg("lon_input")) request->arg("lon_input").toCharArray(Config::lon, sizeof(Config::lon));
    if (request->hasArg("dateformat_input")) request->arg("dateformat_input").toCharArray(Config::dateformat, sizeof(Config::dateformat));
    if (request->hasArg("timeformat_input")) request->arg("timeformat_input").toCharArray(Config::timeformat, sizeof(Config::timeformat));
    if (request->hasArg("miniClockFormat_input")) request->arg("miniClockFormat_input").toCharArray(Config::miniClockFormat, sizeof(Config::miniClockFormat));

    if (request->hasArg("maxcol_input")) Config::maxcol = (uint8_t) request->arg("maxcol_input").toInt();


    // Parse pageOrder string to an array
    if (request->hasArg("pageOrder_input")) {
        const char *pageOrderInput = request->arg("pageOrder_input").c_str(); // Get user input

        uint8_t elemProgress  = 0; // Track which position of the current element we are at
        uint8_t arrayProgress = 0; // Track which element in the array we are working on

        memset(Config::pageOrder, 0, sizeof(Config::pageOrder)); // Clear existing array

        for (uint8_t i = 0; i < strlen(pageOrderInput); i++) {
            char e = pageOrderInput[i]; // Alias the char we are looking at to make using it easier
            
            // Progress to next element in array when comma is detected
            if (e == ',') {
                Config::pageOrder[arrayProgress][elemProgress] = '\0'; // Make sure null byte exists at the end of our element
                arrayProgress++;
                elemProgress = 0; // Reset element progress to start at 0 of next element
                continue;
            }
            
            // Skip empty chars
            if (e == ' ') continue;

            // Ignore any following chars of the current element if we exceeded the available space
            if (elemProgress >= sizeof(Config::pageOrder[arrayProgress]) - 1) continue;

            // Put char into pageOrder where it belongs
            Config::pageOrder[arrayProgress][elemProgress] = e;
            elemProgress++;
        }
    }

    // Parse showuntil string to an array
    if (request->hasArg("showuntil_input")) {
        const char *showuntilInput = request->arg("showuntil_input").c_str(); // Get user input

        uint8_t elemProgress      = 0; // Track which position of the current element we are at
        uint8_t arrayProgress     = 0;  // Track which element in the array we are working on
        char    showuntilTemp[11] = ""; // Temp arr for constructing number before converting to int, UINT32_MAX is 10 chars long

        for (uint8_t i = 0; i < strlen(showuntilInput); i++) {
            char e = showuntilInput[i]; // Alias the char we are looking at to make using it easier
            
            // Progress to next element in array when comma is detected or if we reached the end of the input
            if (e == ',' || i == strlen(showuntilInput) - 1) {
                // Append last char in last iteration, otherwise we are missing the last digit of the last element
                if (e != ',') showuntilTemp[elemProgress] = e;
                
                // Convert showuntilTemp to int and store into Config
                Config::showuntil[arrayProgress] = atoi(showuntilTemp);

                // Clear showuntilTemp
                memset(showuntilTemp, 0, sizeof(showuntilTemp));

                arrayProgress++;
                elemProgress = 0; // Reset element progress to start at 0 of next element
                continue;
            }
            
            // Skip empty chars
            if (e == ' ') continue;

            // Ignore any following chars of the current element if we exceeded the available space
            if (strlen(showuntilTemp) >= 10) continue;

            // Append char to showuntilTemp
            showuntilTemp[elemProgress] = e;
            elemProgress++;
        }
    }


    // If alwaysShowTime is unchecked it isn't included in the body so we must not check for hasArg()
    if (request->arg("alwaysShowTime_input") == "on") Config::alwaysShowTime = true;
        else Config::alwaysShowTime = false;

    if (request->hasArg("clockWeekdaySwitch_input")) Config::clockWeekdaySwitch = (uint16_t) request->arg("clockWeekdaySwitch_input").toInt();


    // Write config changes to fs
    writeConfigToStorage();

    // Let loop display saved message on screen
    savedTime = millis();
}