#include <iostream>
#include "helpers.h"


ESP8266WiFiClass initWifi(LiquidCrystal_PCF8574 lcd, String wifiSSID, String wifiPW, int maxcol, int row) 
{
    clearLine(lcd, maxcol, row); //clear line just to make sure no old characters are left

    lcd.setCursor(3, row);
    lcd.print("Connecting");
    delay(500);

    //Connect to wifi with provided credentials
    WiFi.begin(wifiSSID, wifiPW);

    //Block futher execution until connection is established and show message with waiting animation
    int dots = 0;

    while (WiFi.status() != WL_CONNECTED)
    {
        dots++; //add another dot
        if (dots > 3) dots = 0; //reset when 3 dots are present

        lcd.setCursor(13, row);
        lcd.print(std::string(dots, '.').c_str()); //add dots amount of dots behind "Connecting"
        lcd.print("    "); //Clear any dots that might be behind

        delay(500);
    }


    //Connection established
    centerPrint("Connected!", lcd, maxcol, row, true);
    return WiFi;
}