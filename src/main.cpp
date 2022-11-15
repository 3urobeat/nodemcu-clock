/*
 * File: main.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.11.2022 14:29:29
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "main.h"
#include "../config/config.cpp" // I don't know why I need to include config.cpp here but it otherwise results in an undefined reference to Config::... linker error


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