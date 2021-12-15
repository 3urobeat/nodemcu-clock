/*
 * File: httpsGetJson.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 22:37:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.12.2021 15:05:42
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


void debug(String url, int port, int httpCode, int size, DynamicJsonDocument jsondata)
{
    //Set to true for debugging response in serial monitor
    bool printDebug = true;

    if (printDebug)
    {
        Serial.begin(9600);
        Serial.println("\nPinging \'" + url + "\' on port \'" + (String) port + "\'...");
        Serial.println("Result (Code " + (String) httpCode + ") (Size: " + size + "):");

        String responseStr;
        serializeJson(jsondata, responseStr);

        Serial.println(responseStr);
    }
}


void httpGetJson(String url, DynamicJsonDocument *doc)
{
    WiFiClient client;
    HTTPClient http;

    (*doc).clear();

    int port = 80;

    client.connect(url, port);
    http.useHTTP10(true); //use HTTP 1.0 because getStream() sadly won't work otherwise
    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) deserializeJson(*doc, http.getStream());
        else (*doc).add("http error (" + (String) httpCode + "): " + http.errorToString(httpCode)); //tbh idk what to write here

    http.end(); //Close connection
    client.stop();

    debug(url, port, httpCode, http.getSize(), (*doc));
}


void httpsGetJson(String url, DynamicJsonDocument *doc)
{
    WiFiClientSecure    client;
    HTTPClient          http;
    
    (*doc).clear();

    int port = 443;

    client.setInsecure(); //ignore handshake stuff, we are only getting simple stuff
    client.connect(url, port);

    http.useHTTP10(true); //use HTTP 1.0 because getStream() sadly won't work otherwise
    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) deserializeJson(*doc, http.getStream());
        else (*doc).add("https error (" + (String) httpCode + "): " + http.errorToString(httpCode)); //tbh idk what to write here

    http.end(); //Close connection
    client.stop();

    debug(url, port, httpCode, http.getSize(), (*doc));

}