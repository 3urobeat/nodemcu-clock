/*
 * File: createCustomChars.cpp
 * Project: nodemcu-clock
 * Created Date: 2023-01-15 23:15:22
 * Author: 3urobeat
 *
 * Last Modified: 2025-10-25 15:54:35
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


/**
 * Creates up to 8 custom lcd chars used by the device
 */
void createCustomChars()
{
    // Generator: https://arduinogetstarted.com/tutorials/arduino-lcd#content_custom_character

    // Byte 0: Arrow Up - weather page
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

    // Byte 1: Arrow Down - weather page
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

    // Byte 2: Loading - global
    byte loadingChar[8] = {
        0b01101,
        0b10011,
        0b00111,
        0b00000,
        0b00000,
        0b11100,
        0b11001,
        0b10110
    };

    lcd.createChar(2, loadingChar);

    // Byte 3: Dino - dino page
    byte dinoChar[8] = {
        0b00110,
        0b01011,
        0b01110,
        0b01111,
        0b11100,
        0b11110,
        0b11000,
        0b01100
    };

    lcd.createChar(3, dinoChar);

    // Byte 4: Scrub - dino page
    byte scrubChar[8] = {
        0b00000,
        0b00100,
        0b00101,
        0b10101,
        0b11111,
        0b01110,
        0b00100,
        0b00100
    };

    lcd.createChar(4, scrubChar);

    // Byte 5: Block - general
    byte blockChar[8] = {
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111
    };

    lcd.createChar(5, blockChar); // This should be 255 by default but does not work for me right now
}
