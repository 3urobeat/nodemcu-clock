/*
 * File: config.cpp
 * Project: nodemcu-clock
 * Created Date: 14.11.2022 19:12:41
 * Author: 3urobeat
 * 
 * Last Modified: 14.01.2023 13:10:38
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

    char     pageOrder[3][32]   = { "clock", "weather", "news" };
    uint32_t showuntil[3]       = { 5000, 5000, 0 }; // How long each page should be shown in ms (Special Case: Set to 0 if page should progress on event, news page would progress when headline was fully displayed)
    bool     alwaysShowTime     = true;              // Always show the time in the upper right corner
    uint16_t clockWeekdaySwitch = 2500;              // After how much ms the clock page should switch between date and weekday. Set to 0 to disable

    // Debug settings
    const bool DEBUG    = false; // Logs lots of information about what's currently happening to Serial (Baud 9600)
    const bool IGNOREFS = false; // Ignores loading existing settings from filesystem and overwrites them with the settings above
}