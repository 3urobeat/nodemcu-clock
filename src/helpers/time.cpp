#include <NTPClient.h>
#include <TimeLib.h>

String formatInt(int value)
{
    if (value < 10) return (String) "0" + value;
        else return (String) value;
}

String getDate(NTPClient timeClient, String dateformat)
{
    unsigned long epoch = timeClient.getEpochTime(); 

    //make copy from original value to not overwrite it
    String date = dateformat;

    date.replace("dd", formatInt(day(epoch)));
    date.replace("mm", formatInt(month(epoch)));
    date.replace("yyyy", formatInt(year(epoch)));

    return date;
}

//Provide function to help construct a mini clock that will be called by main.cpp when alwaysShowTime is true
String getTime(NTPClient timeClient, String timeformat)
{
    unsigned long epoch = timeClient.getEpochTime(); 

    //make copy from original value to not overwrite it
    String time = timeformat;

    time.replace("hh", formatInt(hour(epoch)));
    time.replace("mm", formatInt(minute(epoch)));
    time.replace("ss", formatInt(second(epoch)));

    return time;
}