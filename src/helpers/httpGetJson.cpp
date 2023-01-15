/*
 * File: httpGetJson.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 22:37:00
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 14:45:48
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


/**
 * Sends HTTP GET request to an URL and parses JSON data via streaming from it
 * @param url The URL to request data from
 * @param handler Reference to JsonHandler object of class you set up for this request which parses the data you are interested in
 */
void httpGetJson(const char *url, JsonHandler* handler)
{
    // Log request if DEBUG mode is enabled
    #ifdef CLOCK_DEBUG
        Serial.print("httpGetJson(): Sending GET request to: ");
        Serial.println(url);

        debugMemory(F("httpGetJson(): Creating new lib objects..."));
    #endif

    // Create lib obj and set handler // TODO: Maybe move http and client to top scope and let them live forever if they should cause heap fragmentation later on
    HTTPClient *http = new HTTPClient();
    WiFiClient *client = new WiFiClient();
    ArudinoStreamParser *parserLib = new ArudinoStreamParser(); // Specifically create and delete obj manually below again to prevent memory leak (and yes, the lib dev spelled Arduino wrong)

    parserLib->setHandler(handler); // Set our parser as JSON data handler in the lib
    
    // Begin request
    http->setReuse(false);
    http->begin(*client, url);

    // Start connection and send HTTP header
    int httpCode = http->GET();
    debugMemory(F("httpGetJson(): HTTP GET request made, streaming response to parserLib..."));

    // Pass Stream to our parser if request was successful
    if (httpCode == HTTP_CODE_OK) http->writeToStream(parserLib);

    debugMemory(F("httpGetJson(): Finished streaming response"));
    
    // End request
    http->end();
    client->stop();
    delete(parserLib);
    delete(http);
    delete(client);
    debugMemory(F("httpGetJson(): Finished clearing up"));
}