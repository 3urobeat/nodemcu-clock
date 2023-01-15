/*
 * File: config.h
 * Project: nodemcu-clock
 * Created Date: 14.11.2022 19:16:25
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 16:46:48
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#pragma once


// Declare config variables
namespace Config 
{
    extern uint8_t maxcol;

    extern char wifiSSID[5][32];
    extern char wifiPW[5][32];

    extern char setupWifiPW[32];

    extern char lat[8];
    extern char lon[8];

    extern char openweathermaptoken[33];
    extern char newsapitoken[33];

    extern char dateformat[11];
    extern char timeformat[9];
    extern char miniClockFormat[6];

    extern char     pageOrder[3][32];
    extern uint32_t showuntil[3];
    extern bool     alwaysShowTime;
    extern uint16_t pageElementSwitch;
}