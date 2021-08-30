#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "helpers.h"


DynamicJsonDocument httpsGetJson(String url)
{
    WiFiClientSecure client;
    HTTPClient http;

    client.setInsecure(); //ignore handshake stuff, we are only getting the current time
    client.connect(url, 443);

    http.begin(client, url);

    String jsondata;

    if (http.GET() == HTTP_CODE_OK)
    {    
        jsondata = http.getString();
    } else {
        jsondata = "{ \"error\": \"http error\" }"; //tbh idk what to write here
    }

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, jsondata);

    return doc;
}