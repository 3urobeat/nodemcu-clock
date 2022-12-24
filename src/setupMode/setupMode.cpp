/*
 * File: setupMode.cpp
 * Project: setupMode
 * Created Date: 23.12.2022 13:50:55
 * Author: 3urobeat
 * 
 * Last Modified: 24.12.2022 14:08:20
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "setupMode.h"


const uint8_t switchPin = D4;
bool setupModeEnabled = false;


// Initialize setup switch input and check state
bool setupSetupMode()
{
    // Initialize switch pin as input
    pinMode(switchPin, INPUT);

    // Read switch state and update setupModeEnabled
    setupModeEnabled = (digitalRead(switchPin) == HIGH); // Set to true if return is HIGH

    return setupModeEnabled;
}


// Host wifi network and webserver for setup
void hostSetupMode()
{
    lcd.centerPrint("Entering Setup...", 3);   
}


// Handles the running setupMode webserver
void handleSetupMode()
{
    
}