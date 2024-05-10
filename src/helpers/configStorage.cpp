/*
 * File: configStorage.cpp
 * Project: nodemcu-clock
 * Created Date: 2022-12-27 12:28:55
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-10 11:09:47
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


/**
 * Detects first start of device by checking config for missing WiFi credentials. If device is pre-configured, false will be returned.
 * If true, the device should enter setupMode and create config in storage.
 * If false, the device should read config from storage and boot in normal mode.
 */
bool configDetectFirstStart()
{
    // Check for missing Wifi settings in fs to determine first boot (skip check and force overwrite FS if IGNOREFS is enabled)
    #ifndef CLOCK_IGNOREFS
        if (!prefs.isKey("wifiSSID")) {

            // Check if user configured wifi settings at compile time and write to FS
            if (strlen(Config::wifiSSID[0]) > 0) {
                writeConfigToStorage(); // Write compile time config to fs

                return false;
            }

            return true; // Return true if key is not present and nothing was set at compile time
        }
    #else
        debug(F("IGNOREFS is enabled, overwriting FS settings with values found in config.cpp!"));
        writeConfigToStorage();
    #endif

    return false; // Return false if wifiSSID key is present in fs, we assume it has content
}


/**
 * Reads config values from storage and updates Config namespace
 */
void readConfigFromStorage()
{
    Config::maxcol = prefs.getUChar("maxcol");

    prefs.getBytes("wifiSSID", Config::wifiSSID, sizeof(Config::wifiSSID));
    prefs.getBytes("wifiPW", Config::wifiPW, sizeof(Config::wifiPW));
    prefs.getBytes("setupWifiPW", Config::setupWifiPW, sizeof(Config::setupWifiPW));
    prefs.getBytes("lat", Config::lat, sizeof(Config::lat));
    prefs.getBytes("lon", Config::lon, sizeof(Config::lon));
    prefs.getBytes("weatherapitoken", Config::weatherapitoken, sizeof(Config::weatherapitoken));
    prefs.getBytes("newsapitoken", Config::newsapitoken, sizeof(Config::newsapitoken));
    prefs.getBytes("dateformat", Config::dateformat, sizeof(Config::dateformat));
    prefs.getBytes("timeformat", Config::timeformat, sizeof(Config::timeformat));
    prefs.getBytes("miniClockFormat", Config::miniClockFormat, sizeof(Config::miniClockFormat));
    prefs.getBytes("pageOrder", Config::pageOrder, sizeof(Config::pageOrder));
    prefs.getBytes("showuntil", Config::showuntil, sizeof(Config::showuntil));

    Config::alwaysShowTime = prefs.getBool("alwaysShowTime");

    Config::pageElemSwitch = prefs.getUShort("pageElemSwitch");

    debug(F("Config was read from filesystem"));
}


/**
 * Writes config values from Config namespace to storage
 */
void writeConfigToStorage()
{
    // Types doc: https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html#id1
    prefs.putUChar("maxcol", Config::maxcol);

    prefs.putBytes("wifiSSID", Config::wifiSSID, sizeof(Config::wifiSSID));
    prefs.putBytes("wifiPW", Config::wifiPW, sizeof(Config::wifiPW));
    prefs.putBytes("setupWifiPW", Config::setupWifiPW, sizeof(Config::setupWifiPW));
    prefs.putBytes("lat", Config::lat, sizeof(Config::lat));
    prefs.putBytes("lon", Config::lon, sizeof(Config::lon));
    prefs.putBytes("weatherapitoken", Config::weatherapitoken, sizeof(Config::weatherapitoken));
    prefs.putBytes("newsapitoken", Config::newsapitoken, sizeof(Config::newsapitoken));
    prefs.putBytes("dateformat", Config::dateformat, sizeof(Config::dateformat));
    prefs.putBytes("timeformat", Config::timeformat, sizeof(Config::timeformat));
    prefs.putBytes("miniClockFormat", Config::miniClockFormat, sizeof(Config::miniClockFormat));
    prefs.putBytes("pageOrder", Config::pageOrder, sizeof(Config::pageOrder));
    prefs.putBytes("showuntil", Config::showuntil, sizeof(Config::showuntil));

    prefs.putBool("alwaysShowTime", Config::alwaysShowTime);

    prefs.putUShort("pageElemSwitch", Config::pageElemSwitch);

    debug(F("Config was written to filesystem!"));
}
