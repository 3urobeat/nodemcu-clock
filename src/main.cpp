#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

#include "helpers/helpers.h"
#include "pages/pages.h"


//--------- config -----------

int    maxcol   = 20; //width of the display
String wifiSSID = "";
String wifiPW   = "";

String version  = "0.1.0";

//----------------------------


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, maxcol, 4);


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

}

void loop() 
{
    
}