/*
 * File: config.h
 * Project: nodemcu-clock
 * Created Date: 2022-11-14 19:16:25
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-10 11:09:31
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
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

    extern char weatherapitoken[33];
    extern char newsapitoken[33];

    extern char spotifyClientID[33];
    extern char spotifyClientSecret[33];

    extern char dateformat[11];
    extern char timeformat[9];
    extern char miniClockFormat[6];

    extern char     pageOrder[4][32];
    extern uint32_t showuntil[4];
    extern bool     alwaysShowTime;
    extern uint16_t pageElemSwitch;
}
