/*
 * File: spotify.cpp
 * Project: nodemcu-clock
 * Created Date: 17.01.2023 10:39:35
 * Author: 3urobeat
 * 
 * Last Modified: 18.01.2023 23:07:50
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "base64.h"

#include "pages.h"


// Spotify Application ID, secret and redirect URI
const char spotifyClientID[] = "";
const char spotifyClientSecret[] = "";
const char spotifyRedirectUri[] = "http://192.168.55.112/callback";


// Filled by requestAuth() & refreshAccessToken() // TODO: Write into FS, except for expires in timestamp!
char     spotifyAccessToken[256] = ""; // This can get really long
char     spotifyRefreshToken[256] = "";
uint32_t spotifyAccessTokenExpiresTimestamp; // 10/10 var name, very short

bool spotifyRequestAuthWaiting = false;


namespace spotifyPage
{
    // Declare function here and define it later below to reduce clutter while being accessible from setup()
    void refreshCurrentPlayback();

    AsyncWebServer *spotifyAuthWebserver;
    char *authCode;
    
    void requestAuth();
    void fetchAccessToken(const char *code, const char *grantType);

    /**
     * Setup the spotify page
     */
    void setup()
    {
        // Show page title
        lcd.setCursor(0, 0);
        lcd.print("Spotify");

        // Skip page if user has playback paused
    }

    /**
     * Shows the spotify page and gets up-to-date data from the Spotify API
     */
    void update()
    {
        // Check if requestAuth is active and display waiting animation, otherwise get current data from Spotify and display it
        if (spotifyRequestAuthWaiting) {
            static uint8_t animFrame = 0; // Tracking var for animation frame
            
            lcd.animationPrint(lcd.animations.waiting, 5, &animFrame, 12, 3);

            // Check for requestAuth() in a non blocking way if callback was received, clean up and call fetchAccessToken()
            if (authCode[0] != '\0') {
                // Clean up
                spotifyAuthWebserver->reset();
                delete(spotifyAuthWebserver);
                spotifyRequestAuthWaiting = false;

                debug(F("spotify page: Callback processed, clean up done, passing authCode to fetchAccessToken()"));

                // Pass code to fetchAccessToken() to get an accessToken & refreshToken to make requests with
                fetchAccessToken(authCode, "authorization_code");
                delete[] authCode; // Delete authCode when fetchAccessToken() is done
            }

        } else { // Normal operation

            // Check if accessToken is about to expire
            if (millis() + 5000 >= spotifyAccessTokenExpiresTimestamp) {
                fetchAccessToken(spotifyRefreshToken, "refresh_token");
            }
        }
    }

    /**
     * Uses the accessToken to get the current playback
     */
    void refreshCurrentPlayback()
    {
        
    }



    /* --------- Handle Spotify API oAuth --------- */
    // Docs: https://developer.spotify.com/documentation/general/guides/authorization/code-flow/


    /**
     * Hosts a webserver to request auth confirmation from the user, which gets us an auth code.
     * This user interaction is only required once.
     */
    void requestAuth()
    {
        spotifyRequestAuthWaiting = true;

        // Display instructions, goofy formatting to indicate which msg is on which line while saving space
        lcd.setCursor(0, 1); lcd.print("OneTime Auth! Visit:");
        lcd.setCursor(0, 2); lcd.print(WiFi.localIP()); // Get our IP
        lcd.centerPrint("Waiting   ", 3);

        // Block page switch by setting a large pageUpdate
        pageUpdate += 999999;
        
        authCode = new char[256]; // Provide char arr for callback function to copy into, code was in testing 212 chars long
        authCode[0] = '\0'; // Clear at least the first char so that our check in update() works, memsetting the whole thing is not really necessary here


        // Start a webserver on port 80
        spotifyAuthWebserver = new AsyncWebServer(80); // I'm only using the async one as it is already required for setupMode

        // Serves webpage that redirects the user to Spotify
        spotifyAuthWebserver->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            // Construct redirect URL and redirect user
            char url[256] = "https://accounts.spotify.com/authorize/?client_id=";
            char *p = url;

            p = mystrcat(p, spotifyClientID);
            p = mystrcat(p, "&response_type=code&redirect_uri=");
            p = mystrcat(p, spotifyRedirectUri);
            p = mystrcat(p, "&scope=user-read-currently-playing");

            debug(F("spotify page: URL constructed, redirecting user, waiting for callback"));

            // Redirect user to Spotify Auth page
            request->redirect(url);
        });

        // Retrieves the code callback from Spotify
        spotifyAuthWebserver->on("/callback", HTTP_GET, [](AsyncWebServerRequest *request) {
            // Respond to user
            if (request->hasArg("code")) {
                request->send_P(200, "text/plain", "Authentication successful! You may close this page.");

                strncpy(authCode, request->arg("code").c_str(), 256);
            } else {
                request->send_P(500, "text/plain", "Oops - An error occurred! Please try again later.");
            }
        });

        spotifyAuthWebserver->begin();

        debug(F("spotify page: Webserver up, page switch blocked, waiting for request"));


        // update() will constantly check if authCode is populated and clean up
    }


    /**
     * Uses the auth code from requestAuth() to exchange it for a refresh token or refreshes an expired accessToken
     * @param code Authentication Code from requestAuth() callback or refreshToken if accessToken should be refreshed
     * @param grantType "authorization_code" if authCode should be exchanged for an accessToken (by spotifyAuthCallback()), "refresh_token" if accessToken should be refreshed
     */
    void fetchAccessToken(const char *code, const char *grantType)
    {
        // Display loading message as it could take a bit longer
        lcd.clearLine(1);
        lcd.clearLine(3);
        lcd.centerPrint("Refreshing token...", 2, true);


        // Create objects and send post request with code
        WiFiClientSecure *client = new WiFiClientSecure(); // Using WiFiClientSecure costs 10% flash which sucks ass but the request won't work otherwise
        ArudinoStreamParser *parserLib = new ArudinoStreamParser();
        
        SpotifyAccessTokenJsonHandler *parser = new SpotifyAccessTokenJsonHandler(spotifyAccessToken, sizeof(spotifyAccessToken), spotifyRefreshToken, sizeof(spotifyRefreshToken), &spotifyAccessTokenExpiresTimestamp);

        parserLib->setHandler(parser); // Set our parser as JSON data handler in the lib


        /* --------- Construct post request data --------- */
        char postData[312] = "grant_type="; // Length was 292 in testing for grantType authorization_code
        char *postP = postData;
        
        postP = mystrcat(postP, grantType);

        // Depending on if we are requesting an accessToken or just refreshing it we need different parameters
        if (strcmp(grantType, "authorization_code") == 0) postP = mystrcat(postP, "&code=");
            else postP = mystrcat(postP, "&refresh_token=");

        postP = mystrcat(postP, code);

        // Redirect URI is only needed for authorization_code
        if (strcmp(grantType, "authorization_code") == 0) {
            postP = mystrcat(postP, "&redirect_uri=");
            postP = mystrcat(postP, spotifyRedirectUri);
        }

        *(postP) = '\0'; // Make sure there is a null char at the end

        // Get length of our finished postData str so we can include it later in our request
        char postDataLength[6] = "";
        
        itoa(strlen(postData), postDataLength, 10);


        /* --------- Construct authorization header --------- */
        char authStr[128]  = "";

        strcpy(authStr, spotifyClientID);
        strcat(authStr, ":");
        strcat(authStr, spotifyClientSecret);

        strncpy(authStr, base64::encode(authStr, false).c_str(), sizeof(authStr) - 1); // Encode authStr as needed and overwrite authStr to save some memory

        debug(F("spotify page: POST content constructed and objects made"));
        

        /* --------- Construct POST request headers --------- */
        // Construct POST request manually as using HTTPClient always failed (Inspired by: https://github.com/ThingPulse/esp8266-spotify-remote/blob/b1d8c18ed893b6a5a45b26c74c9b73c6625deae8/SpotifyClient.cpp#L221)
        char request[512] = "POST /api/token HTTP/1.1\r\nHost: accounts.spotify.com\r\nAuthorization: Basic ";
        char *reqP = request;

        reqP = mystrcat(reqP, authStr);
        reqP = mystrcat(reqP, "\r\nContent-Length: ");
        reqP = mystrcat(reqP, postDataLength);
        reqP = mystrcat(reqP, "\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection: close\r\n\r\n");
        reqP = mystrcat(reqP, postData);

        debug(F("spotify page: POST request constructed, connecting..."));


        /* --------- Send POST request --------- */
        client->setInsecure();
        client->setNoDelay(false);
        
        if (client->connect("accounts.spotify.com", 443)) { // Only proceed if connection succeeded
            client->print(request); // Send our POST req data over

            // Prepare spotifyAccessTokenExpiresTimestamp, parser will do +=
            spotifyAccessTokenExpiresTimestamp = millis();

            // Send each char we are receiving over to our parser while the connection is alive
            while (client->connected() || client->available()) {
                parserLib->parse((char) client->read());
            }
        } else {
            lcd.centerPrint("Request failed!", 2, true);
        }

        debug(F("spotify page: Connection closed, cleaning up..."));


        /* --------- Clean Up --------- */
        client->stop();
        delete(client);
        delete(parserLib);
        delete(parser);
        lcd.clearLine(2);
        debug(F("spotify page: Finished cleaning up"));

        // Allow page switching again
        pageUpdate = millis();
    }
}