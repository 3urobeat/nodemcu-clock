/*
 * File: backlight.cpp
 * Project: nodemcu-clock
 * Created Date: 2024-05-12 10:03:02
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-12 10:30:24
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


bool backlightEnabled = false;

#ifdef ESP8266
    const uint8_t switchPin = D3;
#elif ESP32
    const uint8_t switchPin = 19;
#endif


/**
 * Checks state of the backlight switch and controls the screen accordingly
 */
void handleBacklight()
{
    bool pinState = (digitalRead(switchPin) != LOW);

    if (pinState != backlightEnabled) {
        lcd.setBacklight(pinState);

        backlightEnabled = pinState;
    }
}


/**
 * Inits the backlight switch pin and calls handleBacklight once
 */
void setupBacklight()
{
    // Initialize switch pin as input
    pinMode(switchPin, INPUT_PULLUP);

    handleBacklight();
}
