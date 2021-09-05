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