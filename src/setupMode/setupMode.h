/*
 * File: setupMode.h
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:51:00
 * Author: 3urobeat
 * 
 * Last Modified: 04.01.2023 12:29:24
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#pragma once

#include <ESPAsyncWebServer.h>
#include "helpers/helpers.h" // Include helpers header which also includes main header

extern bool setupModeEnabled;
extern AsyncWebServer webserver;

bool setupModeEnabledCheck(bool forceSetupMode = false);
void setupModeSetup();
void setupModeLoop();

void setupModeWebPage(AsyncWebServerRequest *request);
void setupModeWebPageSave(AsyncWebServerRequest *request);


// setupModeWebPage HTML content
const char webpage[] PROGMEM = R"rawliteral(
<html>
    <head>
        <title>nodemcu-clock Setup Page</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    
    <body>
        <h1>nodemcu-clock Setup Page</h1>
        
        <!-- %variable% in value fields are placeholders that get replaced by processor function before sending to client -->
        <form action="/post" target="hidden-form" method="post">
            <!-- Wifi Settings -->
            <h3>Wifi Settings</h3>
            <div id="wifi_networks">
                <p>Network 1:</p>
                SSID: <input type="text" name="wifiSSID_input1" value="%wifiSSID_input1%"><br>
                Password: <input type="text" name="wifiPW_input1" value="%wifiPW_input1%"><br>
            </div>
            <br>
            <input type="button" id="addWifi_button" value="+ Add another network" onclick="addWifiNetworkToDiv()"><br> <!-- Button to add another SSID and PW field -->
            <br>
            Setup-Wifi Password: <input type="text" name="setupWifiPW_input" value="%setupWifiPW_input%"><br>
            <br><br>

            <!-- API Keys -->
            <h3>API Keys</h3>
            OpenWeatherMap API Key: <input type="text" name="openweathermaptoken_input" value="%openweathermaptoken_input%"><br>
            News API Key: <input type="text" name="newsapitoken_input" value="%newsapitoken_input%"><br>
            <br><br>

            <!-- Localization -->
            <h3>Localization</h3>
            Coordinates - Lateral (optional): <input type="text" name="lat_input" value="%lat_input%"><br>
            Coordinates - Longitudinal (optional): <input type="text" name="lon_input" value="%lon_input%"><br>
            Date Format: <input type="text" name="dateformat_input" value="%dateformat_input%"><br>
            Time Format: <input type="text" name="timeformat_input" value="%timeformat_input%"><br>
            Mini-Clock Format: <input type="text" name="miniClockFormat_input" value="%miniClockFormat_input%"><br>
            <br><br>

            <!-- System config -->
            <h3>System Config:</h3>
            LCD Columns: <input type="text" name="maxcol_input" value="%maxcol_input%"><br>
            Page Order: <input type="text" name="pageOrder_input" value="%pageOrder_input%"><br>
            Page Duration: <input type="text" name="showuntil_input" value="%showuntil_input%"><br>
            Enable Mini-Clock: <input type="checkbox" name="alwaysShowTime_input" %alwaysShowTime_input%><br>
            Clock-Weekday-Switch Delay: <input type="text" name="clockWeekdaySwitch_input" value="%clockWeekdaySwitch_input%"><br>
            <br><br>
            
            <!-- Save button -->
            <input type="submit" value="Save">
        </form>
        <iframe style="display:none" name="hidden-form"></iframe>
    </body>

    <script>
        // TODO: Add amount of wifi networks specified in config on page load
        let wifiNetworksDiv      = document.getElementById("wifi_networks");
        let amountOfWifiNetworks = 2; // TODO

        // TODO: Specify some sort of placeholder array containing placeholder variables for value fields so that processor() can replace them

        for (let i = 2; i <= amountOfWifiNetworks; i++) {
            wifiNetworksDiv.innerHTML += `
                <p>Network ${i}:</p>
                SSID: <input type="text" name="wifiSSID_input${i}" value=""><br>
                Password: <input type="text" name="wifiPW_input${i}" value=""><br>
            `
        }

        // Function that will be called by button onclick
        function addWifiNetworkToDiv() {
            amountOfWifiNetworks++;

            wifiNetworksDiv.innerHTML += `
                <p>Network ${amountOfWifiNetworks}:</p>
                SSID: <input type="text" name="wifiSSID_input${amountOfWifiNetworks}" value=""><br>
                Password: <input type="text" name="wifiPW_input${amountOfWifiNetworks}" value=""><br>
            `
        }
    </script>
</html>
)rawliteral";