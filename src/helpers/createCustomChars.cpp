/*
 * File: createCustomChars.cpp
 * Project: nodemcu-clock
 * Created Date: 2023-01-15 23:15:22
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-11 11:54:48
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
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

    // Byte 1: Arrow Down
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

    // Byte 2: Loading
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
}
