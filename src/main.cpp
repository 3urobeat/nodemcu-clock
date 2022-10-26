/*
 * File: main.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 26.10.2022 15:28:39
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <lcdHelper.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "helpers/helpers.h"
#include "pages/pages.h"


//--------- config -----------

const int maxcol = 20; //width of the display

const char *wifiSSID[2] = { "", "" }; //you can provide multiple networks if you wish
const char *wifiPW[2]   = { "", "" };

char lat[8] = ""; //set your location manually with latitudinal and longitudinal coordinates. If you leave it empty the program will get you general location automatically via your IP.
char lon[8] = "";

const char openweathermaptoken[33] = "";
const char newsapitoken[33]        = "";

const char dateformat[11]      = "dd.mm.yyyy";
const char timeformat[9]       = "hh:mm:ss";
const char miniClockFormat[6]  = "hh:mm";

const char *pageOrder[3]       = { "clock", "weather", "news" };
const int   showuntil[3]       = { 5000, 5000, 30000 }; //how long each page should be shown in ms
const bool  alwaysShowTime     = true; //always show the time in the upper right corner
const int   clockWeekdaySwitch = 2500; //after how much ms the clock page should switch between date and weekday. Set to 0 to disable

//----------------------------


char version[] = "v0.5.2";

char city[64];   //Yes, we can store the longest city names: https://largest.org/geography/city-names/
char country[3]; //two char long country codes
int  timeoffset; //timezone offset in seconds

int           currentPage;           //current page index in pageOrder array
int           oldPage;               //save previous page to determine if we need to call lcd.clear() (useful if user only set one page in pageOrder to avoid blinking every showuntil seconds)
unsigned long pageupdate;            //save timestamp when page was updated in order to keep track of showuntil without blocking the thread with a delay()
bool          hideMiniClock = false; //will be set to true when clockpage is active

lcdHelper<LiquidCrystal_PCF8574> lcd(0x27, maxcol, 4);

WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000); //timeoffset will be added later manually

const size_t ssidamount = sizeof wifiSSID / sizeof wifiSSID[0]; //calculate wifiSSID size to avoid -Wsizeof-array-argument warning


//Setup stuff (will be run on poweron)
void setup()
{

    //Initiate display
    Wire.begin();
    lcd.begin();
    lcd.backlight();

    //Serial.begin(9600);

    //Print startup screen
    lcd.centerPrint("nodemcu-clock", 0, true);
    lcd.centerPrint(version, 1, true);
    delay(1000);

    //Connect to wifi
    initWifi(wifiSSID, wifiPW, ssidamount, maxcol, 3);
    delay(500);

    //Show loading message
    lcd.centerPrint("Loading...", 3, true);

    //geolocate the used IP to get coordinates and the timeoffset
    getLocation(openweathermaptoken, lat, lon, city, country, &timeoffset);

    //start syncing time
    timeClient.begin();

    //set pageupdate to now to avoid first page getting skipped instantly and add 1 second to compensate for loading time
    pageupdate = millis() + 1000;
    lcd.clear();

}


//Helper function to quickly debug memory usage
void debugMemory() {
    Serial.print("getFreeHeap(): ");
    Serial.println(ESP.getFreeHeap());
    Serial.print("getFreeContStack(): ");
    Serial.println(ESP.getFreeContStack());
}


//Call to display mini clock in the top right
char miniClockResult[6];

void miniClock()
{
    if (!hideMiniClock)
    {
        getTime(miniClockResult, timeClient, timeoffset, miniClockFormat);

        lcd.setCursor(maxcol - strlen(miniClockFormat), 0); //set cursor to the very right of the first line
        lcd.print(miniClockResult);
    }
}


//Use pre-configured loop as pagemanager
void loop() 
{

    //let timelib check if it is time to update the time
    timeClient.update();

    //save currentPage
    oldPage = currentPage;

    //update currentPage if showuntil time for this page passed
    if (pageupdate + showuntil[currentPage] <= millis())
    {
        currentPage++;

        //reset currentPage if we exceeded the amount of pages in the array
        if (currentPage + 1 > *(&pageOrder + 1) - pageOrder) currentPage = 0; //credit for array size hack: https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/

        hideMiniClock = false;    //default
        pageupdate    = millis(); //update timestamp

        if (oldPage != currentPage) lcd.clear();
    }

    //debugMemory();

    if (strcmp(pageOrder[currentPage], "clock") == 0) { // why strcmp? https://stackoverflow.com/questions/2603039/warning-comparison-with-string-literals-results-in-unspecified-behaviour
        
        clockpage(timeClient, timeoffset, clockWeekdaySwitch, dateformat, timeformat);
        hideMiniClock = true;

    } else if (strcmp(pageOrder[currentPage], "weather") == 0) {

        weatherpage(openweathermaptoken, lat, lon, city);

    } else if (strcmp(pageOrder[currentPage], "news") == 0) {

        newspage(newsapitoken, showuntil[currentPage], country, timeoffset, miniClockFormat);

    }

    //update miniClock if enabled
    if (alwaysShowTime) miniClock();

    //wait a moment before updating again
    delay(250);

}