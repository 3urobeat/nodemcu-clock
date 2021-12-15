/*
 * File: main.cpp
 * Project: src
 * Created Date: 30.08.2021 11:19:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.12.2021 22:39:13
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include <Arduino.h>
#include <Wire.h>
#include <NoiascaLiquidCrystal.h> // Article (german): https://werner.rothschopf.net/202003_arduino_liquid_crystal_umlaute.htm | Direct download: https://werner.rothschopf.net/2020/NoiascaLiquidCrystal.zip
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "helpers/helpers.h"
#include "pages/pages.h"


//--------- config -----------

int    maxcol = 20; //width of the display

String wifiSSID[] = { "", "" }; //you can provide multiple networks if you wish
String wifiPW[]   = { "", "" };

String lat = ""; //set your location manually with latitudinal and longitudinal coordinates. If you leave it empty the program will get you general location automatically via your IP.
String lon = "";

String openweathermaptoken = "";
String newsapitoken        = "";

String dateformat      = "dd.mm.yyyy";
String timeformat      = "hh:mm:ss";
String miniClockFormat = "hh:mm";

String pageOrder[3]       = { "clock", "weather", "news" };
int    showuntil[3]       = { 5000, 5000, 30000 }; //how long each page should be shown in ms
bool   alwaysShowTime     = true; //always show the time in the upper right corner
int    clockWeekdaySwitch = 2500; //after how much ms the clock page should switch between date and weekday. Set to 0 to disable

//----------------------------


String version = "0.5.0";

String city;
String country;
int    timeoffset; //timezone offset in seconds

int           currentPage;           //current page index in pageOrder array
int           oldPage;               //save previous page to determine if we need to call lcd.clear() (useful if user only set one page in pageOrder to avoid blinking every showuntil seconds)
unsigned long pageupdate;            //save timestamp when page was updated in order to keep track of showuntil without blocking the thread with a delay()
bool          hideMiniClock = false; //will be set to true when clockpage is active

int    lcdCursorPos[2] = { 0, 0 };           //save current cursor position
String lcdContent[4]   = { "", "", "", "" }; //save content of lcd when printing to be able to compare content later

LiquidCrystal_PCF8574 lcd(0x27, maxcol, 4);

WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000); //timeoffset will be added later manually

size_t ssidamount = sizeof wifiSSID / sizeof wifiSSID[0]; //calculate wifiSSID size to avoid -Wsizeof-array-argument warning


//Setup stuff (will be run on poweron)
void setup()
{

    //Initiate display
    Wire.begin();
    lcd.begin();
    lcd.backlight();

    //Serial.begin(9600);

    //Print startup screen
    centerPrint("nodemcu-clock", 0, true);
    centerPrint("v" + version, 1, true);
    delay(1000);

    //Connect to wifi
    initWifi(lcd, wifiSSID, wifiPW, ssidamount, maxcol, 3);
    delay(500);

    //Show loading message
    centerPrint("Loading...", 3, true);

    //geolocate the used IP to get coordinates and the timeoffset
    getLocation(lcd, openweathermaptoken, &lat, &lon, &city, &country, &timeoffset);

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
void miniClock()
{
    if (!hideMiniClock)
    {
        lcdSetCursor(maxcol - miniClockFormat.length(), 0); //set cursor to the very right of the first line

        lcdPrint(getTime(timeClient, timeoffset, miniClockFormat));
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

    //get current page to shorten these ugly if else statements
    String e = pageOrder[currentPage];

    if (e == "clock") {
        
        clockpage(timeClient, timeoffset, clockWeekdaySwitch, dateformat, timeformat);
        hideMiniClock = true;

    } else if (e == "weather") {

        weatherpage(openweathermaptoken, lat, lon, city);

    } else if (e == "news") {

        newspage(newsapitoken, showuntil[currentPage], country, timeoffset, miniClockFormat);

    }

    //update miniClock if enabled
    if (alwaysShowTime) miniClock();

    //wait a moment before updating again
    delay(250);

}