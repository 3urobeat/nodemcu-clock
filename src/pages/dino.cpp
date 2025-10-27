/*
 * File: dino.cpp
 * Project: nodemcu-clock
 * Created Date: 2021-09-01 15:17:00
 * Author: 3urobeat
 *
 * Last Modified: 2025-10-27 21:24:23
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "pages.h"


namespace dinoPage
{
    const char *title = "";
    const int  updateInterval = 250;
    const bool hideMiniClock = false;

    // TODO: Is this still top scope, so cache is on heap?

    // Pre-Made animations, stored in flash (PROGMEM) - Doc: https://docs.arduino.cc/language-reference/en/variables/utilities/PROGMEM/
    // First 3 characters must be spaces to avoid dino clipping into scrub during fadein!
    const char anim_0[64] PROGMEM = "    \x04      \x04     \x04       \x04         \x04      \x04    \x04    \x04   \x04     ";
    const char anim_1[64] PROGMEM = "      \x04   \x04      \x04   \x04     \x04      \x04   \x04     \x04      \x04   \x04     ";
    const char anim_2[64] PROGMEM = "     \x04   \x04    \x04        \x04  \x04     \x04     \x04      \x04    \x04      \x04   ";
    const char anim_3[64] PROGMEM = "    \x04      \x04    \x04    \x04   \x04       \x04        \x04     \x04       \x04     ";

    #define animAmount 4
    const char *const animations[animAmount] PROGMEM = { anim_0, anim_1, anim_2, anim_3 };

    // Buffer for the currently selected animation
    char thisAnimation[64] = "";

    uint8_t  chosenAnimation;
    uint8_t  moveOffset;
    bool     inJump;
    uint32_t gameOver; // Timestamp to skip page after 5 seconds

    /**
     * Setup the dino page
     */
    void setup()
    {
        moveOffset = 0;
        inJump     = false;
        gameOver   = 0;

        // Fade in ground
        lcd.setCursor(0, 3);

        for (uint8_t i = 0; i < Config::maxcol; i++)
        {
            lcd.write((byte) 5); // Full block char
            delay(25);
        }

        // Select a random animation string and copy it from PROGMEM to buffer
        uint8_t chosenAnim = random(0, animAmount);
        strcpy_P(thisAnimation, (char *)pgm_read_ptr(&(animations[chosenAnim])));

        // Debug - Print selected animation
        /* lcd.setCursor(0, 0);
        lcd.print(chosenAnim); */

        // Print first maxcol chars of level
        lcd.setCursor(0, 2);
        lcd.movingPrint(thisAnimation, &moveOffset, Config::maxcol);

        // Fade in Dino 3 steps
        lcd.setCursor(0, 2);
        lcd.write((byte) 3);
        delay(150);

        lcd.setCursor(0, 2);
        lcd.write(' ');
        lcd.write((byte) 3);
        delay(150);

        lcd.setCursor(1, 2);
        lcd.write(' ');
        lcd.write((byte) 3);
        delay(150);

        lcd.setCursor(2, 2);
        lcd.write(' ');
        lcd.write((byte) 3);

        delay(500);
    }

    /**
     * Update the animation. Dino is static (except for jump), ground moves
     */
    void update()
    {
        // Check if gameOver and skip page after 5 seconds
        if (gameOver != 0) {
            if (gameOver + 5000 < millis()) nextPage();
            return;
        }

        // Track offset before movingPrint() call so we can detect a moveOffset reset
        uint8_t oldMoveOffset = moveOffset; // Track offset before movingPrint() call so we can detect a moveOffset reset

        // Move ground
        lcd.setCursor(0, 2);
        lcd.movingPrint(thisAnimation, &moveOffset, Config::maxcol);

        // This pixel or the next one is a scrub? Jump!
        if (thisAnimation[moveOffset + 1] == '\x04' || thisAnimation[moveOffset + 2] == '\x04')
        {
            // Easter Egg: 2% chance of failing to jump (on only one of the two jump triggers)
            if (random(0, 100) < 2 && thisAnimation[moveOffset + 2] == '\x04') // Checking this here again is fine as C/C++ does short-circuit evaluation
            {
                gameOver = millis();
                lcd.centerPrint("Game Over!", 1);
                return;
            }

            lcd.setCursor(3, 1);
            lcd.write((byte) 3);
            inJump = true;
        }
        else // No scrub
        {
            // Last iteration had a jump? Clear pixel
            if (inJump)
            {
                lcd.setCursor(3, 1);
                lcd.write(' ');
                inJump = false;
            }

            // Print dino which was cleared by ground move
            lcd.setCursor(3, 2);
            lcd.write((byte) 3);
        }

        // Handle special case of showuntil = 0: Detect if movingPrint() just reset moveOffset as string reached the end and force-Call nextPage()
        if (moveOffset < oldMoveOffset && Config::showuntil[currentPage] == 0) nextPage();
    }
}
