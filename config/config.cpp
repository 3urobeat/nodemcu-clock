/*
 * File: config.cpp
 * Project: nodemcu-clock
 * Created Date: 14.11.2022 19:12:41
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 16:48:28
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


// Define config variables at compile time or leave empty (default) to use setupMode
// Note: ESP8266 Flash needs to be empty for these values to be kept, otherwise the values already in flash will be used!
namespace Config
{
    uint8_t maxcol = 20; // Width of the display

    char wifiSSID[5][32] = { "", "", "", "", "" }; // You can provide up to 5 different networks
    char wifiPW[5][32]   = { "", "", "", "", "" };

    char setupWifiPW[32] = "setuppassword136"; // Password used for hosted wifi network in setup mode

    char lat[8] = ""; // Set your location manually with latitudinal and longitudinal coordinates. If you leave it empty the program will get you general location automatically via your IP.
    char lon[8] = "";

    char openweathermaptoken[33] = "";
    char newsapitoken[33]        = "";

    char dateformat[11]      = "dd.mm.yyyy";
    char timeformat[9]       = "hh:mm:ss";
    char miniClockFormat[6]  = "hh:mm";

    char     pageOrder[4][32]   = { "clock", "weather", "news", "spotify" }; //Set an element to "0" to skip that page
    uint32_t showuntil[4]       = { 5000, 5000, 0, 5000 }; // How long each page should be shown in ms (Special Case: Set to 0 if page should progress on event, news page would progress when headline was fully displayed)
    bool     alwaysShowTime     = true;              // Always show the time in the upper right corner
    uint16_t pageElementSwitch  = 2500;              // After how much ms a page should switch between elements (for example date & weekday on clock page, temp & sunrise on weather page). Set to 0 to disable
}