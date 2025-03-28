/*
 * File: main.cpp
 * Project: nodemcu-clock
 * Created Date: 2021-08-30 11:19:00
 * Author: 3urobeat
 *
 * Last Modified: 2025-03-28 14:48:52
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "main.h"
#include "../config/config.cpp" // I don't know why I need to include config.cpp here but it otherwise results in an undefined reference to Config::... linker error


const char version[] = "v0.10.1";

byte *stack_start; // Measure amount of stack used at start so we can track it later from debug.cpp


/* --------- Call libs  --------- */
lcdHelper<LiquidCrystal_PCF8574> lcd(0x27, Config::maxcol, 4);

WiFiUDP   ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000); //timeoffset will be added later manually
Preferences prefs;



/* --------- Run setup function --------- */
void setup()
{
    // Write current address of the stack to stack_start
    byte stack_now;
    stack_start = &stack_now;

    setupHandler();
}



/* --------- Use loop as page manager  --------- */
void loop()
{
    // Call loopHandler which will handle page loading and switching
    loopHandler();

    // Delay next loop iteration, update relatively fast to allow responsive backlight toggling
    delay(50);
}
