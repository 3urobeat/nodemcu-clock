/*
 * File: createCustomChars.cpp
 * Project: nodemcu-clock
 * Created Date: 15.01.2023 23:15:22
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 23:29:38
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


/**
 * Creates custom lcd chars used by the device
 * - 0: Arrow Up (weather page, sunrise)
 * - 1: Arrow Down (weather page, sunset)
 */
void createCustomChars()
{
    // Generator: https://arduinogetstarted.com/tutorials/arduino-lcd#content_custom_character

    // Byte 0: Arrow Up
    byte arrowUpChar[8] = {
        0b00000,
        0b00100,
        0b01110,
        0b10101,
        0b00100,
        0b00100,
        0b00100,
        0b00000
    };
    
    lcd.createChar(0, arrowUpChar);

    // Byte 0: Arrow Down
    byte arrowDownChar[8] = {
        0b00000,
        0b00100,
        0b00100,
        0b00100,
        0b10101,
        0b01110,
        0b00100,
        0b00000
    };

    lcd.createChar(1, arrowDownChar);
}