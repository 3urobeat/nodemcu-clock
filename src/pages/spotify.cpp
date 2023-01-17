/*
 * File: spotify.cpp
 * Project: nodemcu-clock
 * Created Date: 17.01.2023 10:39:35
 * Author: 3urobeat
 * 
 * Last Modified: 18.01.2023 00:39:52
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"


// Spotify Application ID, secret and redirect URI
const char spotifyClientID[] = "";
const char spotifyClientSecret[] = "";
const char spotifyRedirectUri[] = "http://192.168.55.112/callback";


// Filled by requestAuth() & refreshAccessToken()
char     spotifyAccessToken[128] = "";
char     spotifyRefreshToken[256] = "";
uint32_t spotifyAccessTokenExpiresTimestamp; // 10/10 var name, very short

bool spotifyRequestAuthWaiting = false;


namespace spotifyPage
{
    // Declare function here and define it later below to reduce clutter while being accessible from setup()
    void refreshCurrentPlayback();
    
    void requestAuth();
    void fetchAccessToken(const char *code);
    void refreshAccessToken();

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
        } else {

        }
    }

    /**
     * Uses the refreshToken to get the current playback
     */
    void refreshCurrentPlayback()
    {
        
    }



    /* --------- Handle Spotify API oAuth --------- */

    // Store pointer here so that spotifyAuthCallback() can destroy obj
    AsyncWebServer *spotifyAuthWebserver;

    // Declare requestAuth() subfunctions here to make the code flow more readable
    void requestAuthRedirect(AsyncWebServerRequest *request);
    void spotifyAuthCallback(AsyncWebServerRequest *request);
    
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

        // Start a webserver
        spotifyAuthWebserver = new AsyncWebServer(80); // Init webserver on port 80
    
        spotifyAuthWebserver->on("/", HTTP_GET, requestAuthRedirect);         // Serves webpage that redirects the user to Spotify
        spotifyAuthWebserver->on("/callback", HTTP_GET, spotifyAuthCallback); // Retrieves the code callback from Spotify
        spotifyAuthWebserver->begin();

        debug(F("spotify page: Webserver up, page switch blocked, waiting for request"));
    }

    // "Subfunction" of requestAuth(): Construct URL and redirect user 
    void requestAuthRedirect(AsyncWebServerRequest *request)
    {
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
    }

    // "Subfunction" of requestAuth(): Catch code returned by Spotify and clean up
    void spotifyAuthCallback(AsyncWebServerRequest *request)
    {
        spotifyRequestAuthWaiting = false;

        // Respond to user
        if (request->hasArg("code")) request->send_P(200, "text/plain", "Authentication successful! You may close this page.");
            else request->send_P(500, "text/plain", "Oops - An error occurred! Please try again later.");

        // Clean up
        spotifyAuthWebserver->end();
        delete(spotifyAuthWebserver);

        debug(F("spotify page: Callback processed, page switch unblocked, clean up done"));

        // Pass code to fetchAccessToken() to get an accessToken & refreshToken to make requests with
        fetchAccessToken(request->arg("code").c_str());
    }


    /**
     * Uses the auth code from requestAuth() to exchange it for a refresh token
     * @param code Authentication Code from requestAuth() callback
     */
    void fetchAccessToken(const char *code)
    {
        // Display loading message as it could take a bit longer
        lcd.clearLine(1);
        lcd.clearLine(3);
        lcd.centerPrint("Fetching token...", 2, true);

        debug(F("spotify page: Fetching access token with authentication code"));

        // Allow page switching again
        pageUpdate = millis();
    }


    /**
     * Keeps the accessToken up-to-date by using the refreshToken to get a new accessToken
     */
    void refreshAccessToken()
    {

    }
}