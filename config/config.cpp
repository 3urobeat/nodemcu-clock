/*
 * File: config.cpp
 * Project: nodemcu-clock
 * Created Date: 14.11.2022 19:12:41
 * Author: 3urobeat
 * 
 * Last Modified: 24.12.2022 18:50:07
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


// Define config variables
namespace Config
{
    const uint8_t maxcol = 20; // Width of the display

    const char *wifiSSID[2] = { "", "" }; // You can provide multiple networks if you wish
    const char *wifiPW[2]   = { "", "" };

    const char *setupWifiPW = "setuppassword136"; // Password used for hosted wifi network in setup mode

    char lat[8] = ""; // Set your location manually with latitudinal and longitudinal coordinates. If you leave it empty the program will get you general location automatically via your IP.
    char lon[8] = "";

    const char openweathermaptoken[33] = "";
    const char newsapitoken[33]        = "";

    const char dateformat[11]      = "dd.mm.yyyy";
    const char timeformat[9]       = "hh:mm:ss";
    const char miniClockFormat[6]  = "hh:mm";

    const char    *pageOrder[3]       = { "clock", "weather", "news" };
    const uint32_t showuntil[3]       = { 5000, 5000, 30000 }; // How long each page should be shown in ms
    const bool     alwaysShowTime     = true; // Always show the time in the upper right corner
    const uint16_t clockWeekdaySwitch = 2500; // After how much ms the clock page should switch between date and weekday. Set to 0 to disable

    const bool DEBUG = false;
}