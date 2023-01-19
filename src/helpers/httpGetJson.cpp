/*
 * File: httpGetJson.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 22:37:00
 * Author: 3urobeat
 * 
 * Last Modified: 19.01.2023 20:53:04
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
 * Sends HTTP or HTTPS GET request to an URL and parses JSON data via streaming from it
 * @param host The host to request data from (without http://)
 * @param path The path to request data from (everything after the domain, starting with a /)
 * @param port The port to use (80 for http, 443 for https)
 * @param handler Reference to JsonHandler object of class you set up for this request which parses the data you are interested in
 * @param extraHeaders Optional: Char array containing extra headers for the GET request, separated with "\r\n" (max ~400 chars please)
 * @param parserLib Optional: Pass existing parserLib object reference if you are making frequent requests with the same obj
 */
void httpGetJson(const char *host, const char *path, uint16_t port, JsonHandler *handler, const char *extraHeaders, ArudinoStreamParser *parserLib)
{
    // Log request if DEBUG mode is enabled
    #ifdef CLOCK_DEBUG
        Serial.print("httpGetJson(): Sending GET request to: ");
        Serial.print(host);
        Serial.println(path);

        debug(F("httpGetJson(): Creating new lib objects..."));
    #endif

    // Create correct lib objects and set handler
    void *client;

    if (port == 80) client = new WiFiClient();
        else client = new WiFiClientSecure(); // ...Secure can't do http reqs with setInsecure() smh
    
    bool parserLibMade = false; // Track if we need to delete here

    if (parserLib == NULL) { // Create new handler obj if caller didn't pass one
        debug(F("httpGetJson(): User didn't pass an parserLib obj, creating one..."));

        parserLib = new ArudinoStreamParser(); // Specifically create and delete obj manually below again to prevent memory leak (and yes, the lib dev spelled Arduino wrong)
        parserLibMade = true;

        parserLib->setHandler(handler); // Set our parser as JSON data handler in the lib
    }


    /* --------- Construct GET request headers --------- */
    char request[512] = "GET ";
    char *reqP = request;

    reqP = mystrcat(reqP, path);
    reqP = mystrcat(reqP, " HTTP/1.1\r\nHost: ");
    reqP = mystrcat(reqP, host);
    reqP = mystrcat(reqP, "\r\n");

    // Add headers if headersArr is not empty
    if (extraHeaders != NULL) reqP = mystrcat(reqP, extraHeaders);

    reqP = mystrcat(reqP, "Connection: close\r\n\r\n");

    debug(F("httpGetJson(): GET request constructed, connecting..."));
    

    /* --------- Send POST request with correct class type --------- */
    if (port == 80) {
        if (((WiFiClient*) client)->connect(host, port)) { // Only proceed if connection succeeded
            ((WiFiClient*) client)->print(request); // Send our GET req data over

            // Send each char we are receiving over to our parser while the connection is alive
            while (((WiFiClient*) client)->connected() || ((WiFiClient*) client)->available()) {
                parserLib->parse((char) ((WiFiClient*) client)->read());
            }
        } else {
            debug(F("httpGetJson(): HTTP Request failed!"));
        }
    } else {
        ((WiFiClientSecure*) client)->setInsecure();

        if (((WiFiClientSecure*) client)->connect(host, port)) { // Only proceed if connection succeeded
            ((WiFiClientSecure*) client)->print(request); // Send our GET req data over

            // Send each char we are receiving over to our parser while the connection is alive
            while (((WiFiClientSecure*) client)->connected() || ((WiFiClientSecure*) client)->available()) {
                parserLib->parse((char) ((WiFiClientSecure*) client)->read());
            }
        } else {
            debug(F("httpGetJson(): HTTPS Request failed!"));
        }
    }
    
    debug(F("httpGetJson(): Connection closed, cleaning up..."));
    

    /* --------- Clean Up --------- */
    if (port == 80) {
        ((WiFiClient*) client)->stop();
        delete((WiFiClient*) client);
    } else {
        ((WiFiClientSecure*) client)->stop();
        delete((WiFiClientSecure*) client);
    }

    if (parserLibMade) delete(parserLib);
    debug(F("httpGetJson(): Finished cleaning up"));
}