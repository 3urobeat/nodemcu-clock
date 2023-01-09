/*
 * File: configStorage.cpp
 * Project: nodemcu-clock
 * Created Date: 27.12.2022 12:28:55
 * Author: 3urobeat
 * 
 * Last Modified: 09.01.2023 16:35:26
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


/**
 * Detects first start of device by checking config for missing WiFi credentials. If device is pre-configured, false will be returned.
 * If true, the device should enter setupMode and create config in storage.
 * If false, the device should read config from storage and boot in normal mode.
 */
bool configDetectFirstStart()
{
    // Check for missing Wifi settings in fs to determine first boot
    if (!prefs.isKey("wifiSSID")) {

        // Check if user configured wifi settings at compile time and write to fs
        if (strlen(Config::wifiSSID[0]) > 0) {
            writeConfigToStorage(); // Write compile time config to fs

            return false;
        }

        return true; // Return true if key is not present and nothing was set at compile time
    }
    
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
    prefs.getBytes("openweathermaptoken", Config::openweathermaptoken, sizeof(Config::openweathermaptoken));
    prefs.getBytes("newsapitoken", Config::newsapitoken, sizeof(Config::newsapitoken));
    prefs.getBytes("dateformat", Config::dateformat, sizeof(Config::dateformat));
    prefs.getBytes("timeformat", Config::timeformat, sizeof(Config::timeformat));
    prefs.getBytes("miniClockFormat", Config::miniClockFormat, sizeof(Config::miniClockFormat));
    prefs.getBytes("pageOrder", Config::pageOrder, sizeof(Config::pageOrder));
    prefs.getBytes("showuntil", Config::showuntil, sizeof(Config::showuntil));

    Config::alwaysShowTime = prefs.getBool("alwaysShowTime");

    Config::clockWeekdaySwitch = prefs.getUShort("clockWeekdaySwitch");
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
    prefs.putBytes("openweathermaptoken", Config::openweathermaptoken, sizeof(Config::openweathermaptoken));
    prefs.putBytes("newsapitoken", Config::newsapitoken, sizeof(Config::newsapitoken));
    prefs.putBytes("dateformat", Config::dateformat, sizeof(Config::dateformat));
    prefs.putBytes("timeformat", Config::timeformat, sizeof(Config::timeformat));
    prefs.putBytes("miniClockFormat", Config::miniClockFormat, sizeof(Config::miniClockFormat));
    prefs.putBytes("pageOrder", Config::pageOrder, sizeof(Config::pageOrder));
    prefs.putBytes("showuntil", Config::showuntil, sizeof(Config::showuntil));

    prefs.putBool("alwaysShowTime", Config::alwaysShowTime);

    prefs.putUShort("clockWeekdaySwitch", Config::clockWeekdaySwitch);
}