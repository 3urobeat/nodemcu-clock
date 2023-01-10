/*
 * File: setupMode.h
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:51:00
 * Author: 3urobeat
 * 
 * Last Modified: 10.01.2023 15:01:09
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
extern uint32_t savedTime;

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
                <div id="wifi_network1">
                    <p>Network 1:</p>
                    SSID: <input type="text" name="wifiSSID_input1" value="%wifiSSID1%"><br>
                    Password: <input type="text" name="wifiPW_input1" value="%wifiPW1%"><br>
                </div>
            </div>
            <br>
            <input type="button" id="addWifi_button" value="+ Add another network" onclick="addWifiNetworkToDiv()"><br> <!-- Button to add another SSID and PW field -->
            <p id="addWifi_button_text"></p>
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
        /* ------------ SETUP ------------ */
        let wifiNetworksDiv      = document.getElementById("wifi_networks");
        let wifiNetworksIncluded = [ ["%wifiSSID1%", "%wifiPW1%"], ["%wifiSSID2%", "%wifiPW2%"], ["%wifiSSID3%", "%wifiPW3%"], ["%wifiSSID4%", "%wifiPW4%"], ["%wifiSSID5%", "%wifiPW5%"] ]; // This array gets filled by processor when sending page - Needs to be expanded if config changes!

        // Fill "addWifi_button_text" with content dynamically
        document.getElementById("addWifi_button_text").innerHTML = `You can add up to ${wifiNetworksIncluded.length} different wifi networks by clicking the button above!`;

        // Append fields for available network slot, hidden by default if empty. This ensures that the network fields order stays correct even when network 1 and 3 are filled but not 2
        for (let i = 2; i <= wifiNetworksIncluded.length; i++) {
            // Add "hidden" to div if this network entry is empty
            wifiNetworksDiv.innerHTML += `
                <div id="wifi_network${i}" ${wifiNetworksIncluded[i - 1][0] ? "" : "hidden"}>
                    <p>Network ${i}:</p>
                    SSID: <input type="text" name="wifiSSID_input${i}" value="${wifiNetworksIncluded[i - 1][0]}"><br>
                    Password: <input type="text" name="wifiPW_input${i}" value="${wifiNetworksIncluded[i - 1][1]}"><br>
                </div>
            `

            // Check if all divs are visible in the last iteration as we reached the max amount of allowed wifi networks
            if (i == wifiNetworksIncluded.length) {
                if (Array.from(wifiNetworksDiv.children).every(e => !e.hidden)) document.getElementById("addWifi_button").disabled = true;
            }
        }


        /* ------------ EVENTS ------------ */

        // Unhide first hidden div found inside wifi_networks div when user clicks addWifi_button
        function addWifiNetworkToDiv() {
            for (let i = 0; i < wifiNetworksDiv.children.length; i++) {
                // Unhide this child if hidden and break loop
                if (wifiNetworksDiv.children[i].hidden) {
                    wifiNetworksDiv.children[i].hidden = false;
                    break;
                }
            }

            // Disable button if we reached the max amount of allowed wifi networks, aka all divs are visible
            if (Array.from(wifiNetworksDiv.children).every(e => !e.hidden)) document.getElementById("addWifi_button").disabled = true;
        }
    </script>
</html>
)rawliteral";