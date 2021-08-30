#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#include "./helpers/helpers.h"


//--------- config -----------

int    maxcol   = 20; //width of the display

String version  = "0.1.0";

//----------------------------


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);


void setup() {

    //Initiate display
    lcd.init();
    lcd.backlight();

    //Print startup screen
    centerPrint("nodemcu-clock", lcd, maxcol, 0);
    centerPrint("v" + version, lcd, maxcol, 1);
    delay(1000);

}

void loop() {
    
}