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

String dateformat = "dd.mm.yyyy";
String timeformat = "hh:mm:ss";

int    showuntil      = 5000; //how long a page should be shown in ms

String version = "0.1.0";

//----------------------------


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, maxcol, 4);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", timeoffset, 60000);


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
    ESP8266WiFiClass WiFi = initWifi(lcd, wifiSSID, wifiPW, maxcol, 3);
    //centerPrint(WiFi.localIP().toString(), lcd, maxcol, 2, true); //print ip for testing
    delay(500);

    centerPrint("Loading...", lcd, maxcol, 3, true);

    timeClient.begin(); //start syncing time

}


//Use pre-configured loop as pagemanager
void loop() 
{

    //Show first page
    clockpage(lcd, timeClient, dateformat, timeformat, maxcol, millis() + showuntil);

}