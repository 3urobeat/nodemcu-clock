/*
 * File: main.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.11.2022 13:23:20
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "main.h"


// TODO: Move config to separate file
/* --------- Config variables  --------- */
namespace Config
{
    const int maxcol = 20; // Width of the display

    const char *wifiSSID[2] = { "", "" }; // You can provide multiple networks if you wish
    const char *wifiPW[2]   = { "", "" };

    char lat[8] = ""; // Set your location manually with latitudinal and longitudinal coordinates. If you leave it empty the program will get you general location automatically via your IP.
    char lon[8] = "";

    const char openweathermaptoken[33] = "";
    const char newsapitoken[33]        = "";

    const char dateformat[11]      = "dd.mm.yyyy";
    const char timeformat[9]       = "hh:mm:ss";
    const char miniClockFormat[6]  = "hh:mm";

    const char *pageOrder[3]       = { "clock", "weather", "news" };
    const int   showuntil[3]       = { 5000, 5000, 30000 }; // How long each page should be shown in ms
    const bool  alwaysShowTime     = true; // Always show the time in the upper right corner
    const int   clockWeekdaySwitch = 2500; // After how much ms the clock page should switch between date and weekday. Set to 0 to disable

    const bool DEBUG = false;
}

const char version[] = "v0.5.2";



/* --------- Call libs  --------- */
lcdHelper<LiquidCrystal_PCF8574> lcd(0x27, Config::maxcol, 4);

WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000); //timeoffset will be added later manually



/* --------- Run setup function --------- */
void setup()
{
    setupHandler();
}



/* --------- Use loop as page manager  --------- */
void loop()
{
    // Call loopHandler which will handle page loading and switching
    loopHandler();

    // Delay next loop iteration, updating every 250ms is fast enough
    delay(250);
}