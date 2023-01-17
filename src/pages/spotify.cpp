/*
 * File: spotify.cpp
 * Project: nodemcu-clock
 * Created Date: 17.01.2023 10:39:35
 * Author: 3urobeat
 * 
 * Last Modified: 17.01.2023 23:26:09
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#include "pages.h"


// Spotify Application ID & Secret
const char spotifyClientID[] = "";
const char spotifyClientSecret[] = "";


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
    void fetchAccessToken();
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
        lcd.setCursor(0, 2); lcd.print(WiFi.localIP());
        lcd.centerPrint("Waiting   ", 3);

        // Block page switch by setting a large pageUpdate
        pageUpdate += 999999;

        // Start a webserver
        spotifyAuthWebserver = new AsyncWebServer(80); // Init webserver on port 80
    
        spotifyAuthWebserver->on("/", HTTP_GET, requestAuthRedirect);         // Serves webpage that redirects the user to Spotify
        spotifyAuthWebserver->on("/callback", HTTP_GET, spotifyAuthCallback); // Retrieves the code callback from Spotify
        spotifyAuthWebserver->begin();

        debugMemory(F("spotify page: Webserver up, page switch blocked, waiting for callback"));
    }

    // "Subfunction" of requestAuth(): Construct URL and redirect user 
    void requestAuthRedirect(AsyncWebServerRequest *request)
    {
        // Redirect user to Spotify Auth page
        request->send(200, "text", "Hello");
    }

    // "Subfunction" of requestAuth(): Catch code returned by Spotify and clean up
    void spotifyAuthCallback(AsyncWebServerRequest *request)
    {
        spotifyRequestAuthWaiting = false;

        // Clean up
        delete(spotifyAuthWebserver);

        // Allow page switching in 10 seconds again
        pageUpdate = millis() + 10000;
    }


    /**
     * Uses the auth code from requestAuth() to exchange it for a refresh token
     */
    void fetchAccessToken()
    {
        
    }

    /**
     * Keeps the accessToken up-to-date by using the refreshToken to get a new accessToken
     */
    void refreshAccessToken()
    {

    }
}