/*
 * File: httpsGetJson.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 22:37:00
 * Author: 3urobeat
 * 
 * Last Modified: 31.12.2021 15:52:58
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "helpers.h"


void debug(const char *url, const char *port, int httpCode, int size, DynamicJsonDocument jsondata)
{
    Serial.begin(9600);

    Serial.print("\nPinging \'");
    Serial.print(url);
    Serial.print("\' on port \'");
    Serial.print(port);
    Serial.print("\'...\n");

    Serial.print("Result (Code ");
    Serial.print(httpCode);
    Serial.print(") (Size: ");
    Serial.print(size);
    Serial.print("):\n");

    char responseStr[256];
    serializeJson(jsondata, responseStr);

    Serial.println(responseStr);
}


void httpGetJson(const char *url, DynamicJsonDocument *doc, StaticJsonDocument<128> filter)
{
    WiFiClient client;
    HTTPClient http;

    (*doc).clear();

    client.connect(url, 80);
    http.useHTTP10(true); //use HTTP 1.0 because getStream() sadly won't work otherwise
    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        deserializeJson(*doc, http.getStream(), DeserializationOption::Filter(filter));
    } else {
        char fullStr[64] = "http error(";
        char *p = fullStr;

        //Convert httpCode to string
        char buf[5];
        itoa(httpCode, buf, 10);

        p = mystrcat(fullStr, buf);
        p = mystrcat(fullStr, ")");
        *(p) = '\0'; //add null char to the end

        (*doc).add(fullStr);
    }

    http.end(); //Close connection
    client.stop();

    //debug(url, "80", httpCode, http.getSize(), (*doc));
}


void httpsGetJson(const char *url, DynamicJsonDocument *doc, StaticJsonDocument<128> filter)
{
    WiFiClientSecure client;
    HTTPClient       http;
    
    (*doc).clear();

    client.setInsecure(); //ignore handshake stuff, we are only getting simple stuff
    client.connect(url, 443);

    http.useHTTP10(true); //use HTTP 1.0 because getStream() sadly won't work otherwise
    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        deserializeJson(*doc, http.getStream(), DeserializationOption::Filter(filter));
    } else {
        char fullStr[64] = "https error(";
        char *p = fullStr;

        //Convert httpCode to string
        char buf[5];
        itoa(httpCode, buf, 10);

        p = mystrcat(fullStr, buf);
        p = mystrcat(fullStr, ")");
        *(p) = '\0'; //add null char to the end

        (*doc).add(fullStr);
    }
    
    http.end(); //Close connection
    client.stop();

    //debug(url, "443", httpCode, http.getSize(), (*doc));

}