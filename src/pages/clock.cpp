#include <TimeLib.h>

#include "pages.h"
#include "helpers/helpers.h"

String formatInt(int value)
{
    if (value < 10) return (String) "0" + value;
        else return (String) value;
}

void clockpage(LiquidCrystal_I2C lcd, NTPClient timeClient, String dateformat, String timeformat, int maxcol)
{
    lcd.clear();

    while (true)
    {
        timeClient.update();
        unsigned long epoch = timeClient.getEpochTime(); 

        //make copy from original values to not overwrite them
        String date = dateformat;
        String time = timeformat;

        date.replace("dd", formatInt(day(epoch)));
        date.replace("mm", formatInt(month(epoch)));
        date.replace("yyyy", formatInt(year(epoch)));

        time.replace("hh", formatInt(hour(epoch)));
        time.replace("mm", formatInt(minute(epoch)));
        time.replace("ss", formatInt(second(epoch)));

        centerPrint(time.c_str(), lcd, maxcol, 1, false);
        centerPrint(date.c_str(), lcd, maxcol, 2, false);

        delay(1000);
    }
}