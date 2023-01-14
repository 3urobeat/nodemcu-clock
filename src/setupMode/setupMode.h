/*
 * File: setupMode.h
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:51:00
 * Author: 3urobeat
 * 
 * Last Modified: 14.01.2023 19:34:36
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

extern bool     setupModeEnabled;
extern uint32_t savedTime;

bool setupModeEnabledCheck(bool forceSetupMode = false);
void setupModeSetup();
void setupModeLoop();

void setupModeWebPage(AsyncWebServerRequest *request);
void setupModeWebPageSave(AsyncWebServerRequest *request);


// setupModeWebPage HTML content, compressed to use less space, please see setupModeWebPage.html for full
const char webpage[] PROGMEM = R"rawliteral(
<html>
<head><title>nodemcu-clock Setup Page</title><meta name="viewport" content="width=device-width,initial-scale=1"></head>
<body><h1>nodemcu-clock Setup Page</h1><form action="/post" target="hidden-form" method="post"><h3>Wifi Settings</h3><div id="wifi_networks"><div id="wifi_network1"><p>Network 1:</p>SSID: <input type="text" name="wifiSSID_input1" value="%wifiSSID1%"><br>Password: <input type="text" name="wifiPW_input1" value="%wifiPW1%"><br></div></div><br><input type="button" id="addWifi_button" value="+ Add another network" onclick="addWifiNetworkToDiv()"><br><p id="addWifi_button_text"></p><br>Setup-Wifi Password: <input type="text" name="setupWifiPW_input" value="%setupWifiPW_input%"><br><br><br><h3>API Keys</h3>OpenWeatherMap API Key: <input type="text" name="openweathermaptoken_input" value="%openweathermaptoken_input%"><br>News API Key: <input type="text" name="newsapitoken_input" value="%newsapitoken_input%"><br><br><br><h3>Localization</h3>Coordinates - Lateral (optional): <input type="text" name="lat_input" value="%lat_input%"><br>Coordinates - Longitudinal (optional): <input type="text" name="lon_input" value="%lon_input%"><br>Date Format: <input type="text" name="dateformat_input" value="%dateformat_input%"><br>Time Format: <input type="text" name="timeformat_input" value="%timeformat_input%"><br>Mini-Clock Format: <input type="text" name="miniClockFormat_input" value="%miniClockFormat_input%"><br><br><br><h3>System Config:</h3>LCD Columns: <input type="text" name="maxcol_input" value="%maxcol_input%"><br>Page Order: <input type="text" name="pageOrder_input" value="%pageOrder_input%"><br>Page Duration: <input type="text" name="showuntil_input" value="%showuntil_input%"><br>Enable Mini-Clock: <input type="checkbox" name="alwaysShowTime_input" %alwaysshowtime_input%><br>Clock-Weekday-Switch Delay: <input type="text" name="clockWeekdaySwitch_input" value="%clockWeekdaySwitch_input%"><br><br><br><input type="submit" value="Save"></form><iframe style="display:none" name="hidden-form"></iframe></body>
<script>let wifiNetworksDiv=document.getElementById("wifi_networks"),wifiNetworksIncluded=[["%wifiSSID1%","%wifiPW1%"],["%wifiSSID2%","%wifiPW2%"],["%wifiSSID3%","%wifiPW3%"],["%wifiSSID4%","%wifiPW4%"],["%wifiSSID5%","%wifiPW5%"]];document.getElementById("addWifi_button_text").innerHTML=`You can add up to ${wifiNetworksIncluded.length} different wifi networks by clicking the button above!`;for(let a=2;a<=wifiNetworksIncluded.length;a++)wifiNetworksDiv.innerHTML+=`
                <div id="wifi_network${a}" ${wifiNetworksIncluded[a-1][0]?"":"hidden"}>
                    <p>Network ${a}:</p>
                    SSID: <input type="text" name="wifiSSID_input${a}" value="${wifiNetworksIncluded[a-1][0]}"><br>
                    Password: <input type="text" name="wifiPW_input${a}" value="${wifiNetworksIncluded[a-1][1]}"><br>
                </div>
            `,a==wifiNetworksIncluded.length&&Array.from(wifiNetworksDiv.children).every(a=>!a.hidden)&&(document.getElementById("addWifi_button").disabled=!0);function addWifiNetworkToDiv(){for(let a=0;a<wifiNetworksDiv.children.length;a++)if(wifiNetworksDiv.children[a].hidden){wifiNetworksDiv.children[a].hidden=!1;break}Array.from(wifiNetworksDiv.children).every(a=>!a.hidden)&&(document.getElementById("addWifi_button").disabled=!0)}</script></html>
)rawliteral";

// Compress webpage using: https://codebeautify.org/html-compressor & https://jscompress.com/