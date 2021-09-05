#include "pages.h"
#include "helpers/helpers.h"


void clockpage(LiquidCrystal_I2C lcd, NTPClient timeClient, int timeoffset, String dateformat, String timeformat, int maxcol)
{
    centerPrint(getTime(timeClient, timeoffset, timeformat).c_str(), lcd, maxcol, 1, false);
    centerPrint(getDate(timeClient, timeoffset, dateformat).c_str(), lcd, maxcol, 2, false);
}