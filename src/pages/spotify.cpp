/*
 * File: spotify.cpp
 * Project: nodemcu-clock
 * Created Date: 17.01.2023 10:39:35
 * Author: 3urobeat
 * 
 * Last Modified: 17.01.2023 23:18:18
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
char spotifyAccessToken[128] = "";
char spotifyRefreshToken[256] = "";

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
        
    }

    /**
     * Uses the refreshToken to get the current playback
     */
    void refreshCurrentPlayback()
    {
        
    }



    /* --------- Handle Spotify API oAuth --------- */

    /**
     * Hosts a webserver to request auth confirmation from the user, which gets us an auth code
     */
    void requestAuth()
    {

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