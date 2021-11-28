/*
 * File: httpsGetJson.cpp
 * Project: helpers
 * Created Date: 30.08.2021 22:37:00
 * Author: 3urobeat
 * 
 * Last Modified: 28.11.2021 16:30:49
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

DynamicJsonDocument debugAndFormat(String url, int port, int httpCode, String jsondata)
{
    //Set to true for debugging response in serial monitor
    bool printDebug = false;

    if (printDebug)
    {
        Serial.begin(9600);
        Serial.println("\nPinging \'" + url + "\' on port \'" + (String) port + "\'...");
        Serial.println("Result (" + (String) httpCode + "):");
        Serial.println(jsondata);
    }

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, jsondata);

    return doc;
}

DynamicJsonDocument httpGetJson(String url)
{
    WiFiClient       client;
    HTTPClient       http;
    String           jsondata;

    int port = 80;

    client.connect(url, port);
    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {    
        jsondata = http.getString();
    } else {
        jsondata = "{ \"error\": \"http error: " + (String) httpCode + "\"}"; //tbh idk what to write here
    }

    http.end(); //Close connection
    client.stop();

    return debugAndFormat(url, port, httpCode, jsondata);
}

DynamicJsonDocument httpsGetJson(String url)
{
    WiFiClientSecure client;
    HTTPClient       http;
    String           jsondata;

    int port = 443;

    client.setInsecure(); //ignore handshake stuff, we are only getting simple stuff
    client.connect(url, port);
    http.begin(client, url);

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {    
        jsondata = http.getString();
    } else {
        jsondata = "{ \"error\": \"https error: " + (String) httpCode + "\"}"; //tbh idk what to write here
    }

    http.end(); //Close connection
    client.stop();

    return debugAndFormat(url, port, httpCode, jsondata);
}