/*
 * File: httpGetJson.cpp
 * Project: nodemcu-clock
 * Created Date: 30.08.2021 22:37:00
 * Author: 3urobeat
 * 
 * Last Modified: 30.06.2023 09:47:10
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/3urobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
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
 * @returns HTTP Code
 */
int16_t httpGetJson(const char *host, const char *path, uint16_t port, JsonHandler *handler, const char *extraHeaders, ArudinoStreamParser *parserLib)
{
    // Log request if DEBUG mode is enabled
    #ifdef CLOCK_DEBUG
        Serial.print("httpGetJson(): Sending GET request to: ");
        Serial.print(host);
        Serial.println(path);

        debug(F("httpGetJson(): Creating new lib objects..."));
    #endif

    // Create correct lib objects and set handler
    WiFiClient       *wifiClient;
    WiFiClientSecure *wifiSecureClient;

    if (port == 80) wifiClient = new WiFiClient();

    #ifndef CLOCK_NOHTTPS // User can reduce size of image by ~10% if build flag NOHTTPS is enabled
        if (port != 80) wifiSecureClient = new WiFiClientSecure();
    #endif

    
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
    reqP = mystrcat(reqP, "\r\nUser-Agent: ESP8266HTTPClient\r\n");
    *(reqP) = '\0'; // Make sure there is a null char at the end

    // Add headers if headersArr is not empty
    if (extraHeaders != NULL) reqP = mystrcat(reqP, extraHeaders);

    reqP = mystrcat(reqP, "Connection: close\r\n\r\n");

    debug(F("httpGetJson(): GET request constructed, connecting..."));
    

    /* --------- Send POST request with correct class type --------- */
    char buf[4] = "";
    int16_t httpCode = 0;

    if (port == 80) {
        if (wifiClient->connect(host, port)) { // Only proceed if connection succeeded
            wifiClient->print(request); // Send our GET req data over

            // Get HTTP Code
            strncpy(buf, wifiClient->readStringUntil('\r').c_str() + 9, 3); // Substring HTTP Code from "HTTP/1.1 200 OK"
            httpCode = atoi(buf);

            // Send each char we are receiving over to our parser while the connection is alive (unless code 204 No Content was received)
            if (httpCode != 204) {
                while (wifiClient->connected() || wifiClient->available()) {
                    parserLib->parse((char) wifiClient->read());
                }
            }
        } else {
            debug(F("httpGetJson(): HTTP Request failed!"));
        }

    #ifndef CLOCK_NOHTTPS
    } else {
        wifiSecureClient->setInsecure();

        if (wifiSecureClient->connect(host, port)) { // Only proceed if connection succeeded
            wifiSecureClient->print(request); // Send our GET req data over

            // Get HTTP Code
            strncpy(buf, wifiSecureClient->readStringUntil('\r').c_str() + 9, 3); // Substring HTTP Code from "HTTP/1.1 200 OK" to "200"
            httpCode = atoi(buf);

            // Send each char we are receiving over to our parser while the connection is alive (unless code 204 No Content was received)
            if (httpCode != 204) {
                while (wifiSecureClient->connected() || wifiSecureClient->available()) {
                    parserLib->parse((char) wifiSecureClient->read());
                }
            }
        } else {
            debug(F("httpGetJson(): HTTPS Request failed!"));
        }
    #endif
    }
    
    debug(F("httpGetJson(): Connection closed, cleaning up..."));
    

    /* --------- Clean Up --------- */
    if (port == 80) delete(wifiClient);
    #ifndef CLOCK_NOHTTPS
        if (port != 80) delete(wifiSecureClient);
    #endif
    if (parserLibMade) delete(parserLib);
    debug(F("httpGetJson(): Finished cleaning up"));

    return httpCode;
}