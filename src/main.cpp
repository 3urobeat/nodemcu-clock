#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "helpers/helpers.h"
#include "pages/pages.h"


//--------- config -----------

int    maxcol = 20; //width of the display

String wifiSSID = "";
String wifiPW   = "";

int    timeoffset = 7200; //your timezone time offset in seconds

String dateformat      = "dd.mm.yyyy";
String timeformat      = "hh:mm:ss";
String miniClockFormat = "hh:mm";

String pageOrder[1]   = { "clock" };
int    showuntil      = 5000; //how long a page should be shown in ms
bool   alwaysShowTime = true; //always show the time in the upper right corner

String version = "0.1.0";

//----------------------------


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, maxcol, 4);

WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", timeoffset, 60000);

int           currentPage;           //current page index in pageOrder array
int           oldPage;               //save previous page to determine if we need to call lcd.clear() (useful if user only set one page in pageOrder to avoid blinking every showuntil seconds)
unsigned long pageupdate;            //save timestamp when page was updated in order to keep track of showuntil without blocking the thread with a delay()
bool          hideMiniClock = false; //will be set to true when clockpage is active


void miniClock()
{
    if (!hideMiniClock)
    {
        lcd.setCursor(maxcol - miniClockFormat.length(), 0); //set cursor to the very right of the first line

        lcd.print(getTime(timeClient, miniClockFormat));
    }
}


void setup()
{

    //Initiate display
    lcd.init();
    lcd.backlight();

    //Print startup screen
    centerPrint("nodemcu-clock", lcd, maxcol, 0, true);
    centerPrint("v" + version, lcd, maxcol, 1, true);
    delay(1000);

    //Connect to wifi
    initWifi(lcd, wifiSSID, wifiPW, maxcol, 3);
    delay(500);

    //Show loading message
    centerPrint("Loading...", lcd, maxcol, 3, true);

    //start syncing time
    timeClient.begin();

    //set pageupdate to now to avoid first page getting skipped instantly and add 1 second to compensate for loading time
    pageupdate = millis() + 1000;
    lcd.clear();

}


//Use pre-configured loop as pagemanager
void loop() 
{

    //let timelib check if it is time to update the time
    timeClient.update();

    //save currentPage
    oldPage = currentPage;

    //update currentPage if showuntil time passed
    if (pageupdate + showuntil <= millis())
    {
        currentPage++;

        //reset currentPage if we exceeded the amount of pages in the array
        if (currentPage + 1 > *(&pageOrder + 1) - pageOrder) currentPage = 0; //credit for array size hack: https://www.geeksforgeeks.org/how-to-find-size-of-array-in-cc-without-using-sizeof-operator/

        hideMiniClock = false; //default
        pageupdate = millis(); //update timestamp

        if (oldPage != currentPage) lcd.clear();
    }

    //get current page to shorten these ugly if else statements
    String e = pageOrder[currentPage];

    if (e == "clock") {
        
        clockpage(lcd, timeClient, dateformat, timeformat, maxcol);
        hideMiniClock = true;

    }

    //update miniClock if enabled
    if (alwaysShowTime) miniClock();

    //we only need to update every second
    delay(1000);

}