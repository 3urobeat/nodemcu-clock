/*
 * File: flappy.cpp
 * Project: nodemcu-clock
 * Created Date: 2021-09-01 15:17:00
 * Author: 3urobeat
 *
 * Last Modified: 2025-10-26 20:23:08
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "pages.h"


namespace flappyPage
{
    const char *title = "";
    const int  updateInterval = 250;
    const bool hideMiniClock = false;

    // TODO: Is this still top scope, so cache is on heap?

    // Pre-Made animations, stored in flash (PROGMEM) - Doc: https://docs.arduino.cc/language-reference/en/variables/utilities/PROGMEM/
    // First 3 characters must be spaces to avoid bird clipping into pipe during fadein!
    // Number indicates the row, starting with 0 as the first one, where the pipe opening will be. Avoid really steep inclines to avoid glitches!
    const char anim_0[64] PROGMEM = "    2      0     1       0         3      1    2    3   2     ";
    const char anim_1[64] PROGMEM = "      1   2      0   1     3      1   0     2      3   2     ";
    const char anim_2[64] PROGMEM = "     3   2    1        3  2     0     2      1    2      3   ";
    const char anim_3[64] PROGMEM = "    2      3    2    1   2       0        3     2       0     ";

    #define animAmount 4
    const char *const animations[animAmount] PROGMEM = { anim_0, anim_1, anim_2, anim_3 };

    // Buffer for the currently selected animation
    char thisAnimation[64] = "";

    uint8_t  chosenAnimation;
    uint8_t  moveOffset;
    uint8_t  currentBirdRow;

    /**
     * Setup the flappy page
     */
    void setup()
    {
        // Select a random animation string and copy it from PROGMEM to buffer
        uint8_t chosenAnim = random(0, animAmount);
        strcpy_P(thisAnimation, (char *)pgm_read_ptr(&(animations[chosenAnim])));

        moveOffset     = 0;
        currentBirdRow = thisAnimation[4] != ' ' ? thisAnimation[4] - '0' : 1; // Set bird position to hole row if first hole is closer than bird can move to

        // Print first maxcol chars of level
        for (uint8_t col = 0; col < Config::maxcol; col++)
        {
            char thisChar = thisAnimation[col];

            // Check if this char indicates a pipe. If yes, print full block char for every row except the opening
            if (thisChar != ' ')
            {
                for (uint8_t row = (col >= Config::maxcol - 5 ? 1 : 0); row < displayRows; row++) // Don't overwrite miniClock in first row, causes unnecessary blinking
                {
                    if (thisChar - '0' != row) // Compare char representing number with number
                    {
                        lcd.setCursor(col, row);
                        lcd.write((byte) 5); // Full block char
                    }
                }
            }
        }

        moveOffset++;

        // Fade in bird 2 steps
        lcd.setCursor(0, currentBirdRow);
        lcd.write((byte) 3);
        delay(150);

        lcd.setCursor(0, currentBirdRow);
        lcd.write(' ');
        lcd.write((byte) 3);
        delay(150);

        lcd.setCursor(1, currentBirdRow);
        lcd.write(' ');
        lcd.write((byte) 3);

        delay(500);
    }

    /**
     * Update the animation. Bird is static on x axis (column 2), ground moves
     */
    void update()
    {
        // Move background
        for (uint8_t col = 0; col < Config::maxcol; col++)
        {
            char thisChar = thisAnimation[col + moveOffset];

            for (uint8_t row = (col >= Config::maxcol - 5 ? 1 : 0); row < displayRows; row++) // Don't overwrite miniClock in first row, causes unnecessary blinking
            {
                lcd.setCursor(col, row);

                // Print pipe if not empty and not matching row indicating hole position
                if (thisChar != ' ' && thisChar - '0' != row) // Compare char representing number with number
                {
                    lcd.write((byte) 5); // Full block char
                }
                else
                {
                    lcd.write(' ');
                }
            }
        }

        // Make a movement decision - Look ahead 4..2 columns, move bird towards hole if bird column offset to hole matches lookahead - 1. This should result in a neat linear ascend/descend
        // We look ahead 4 columns to reach hole row one column earlier to counteract display ghosting. This requires lookahead - 1, as our display has 3 rows (counting from 0) but lookahead can be 4.
        for (uint8_t colLookAhead = 4; colLookAhead > 1; colLookAhead--)
        {
            if (thisAnimation[moveOffset + 2 + colLookAhead] != ' ')
            {
                // Calculate offset of bird (at x == 2) to pipe hole
                int8_t rowDifference = (thisAnimation[moveOffset + 2 + colLookAhead] - '0') - currentBirdRow;

                if (abs(rowDifference) == colLookAhead - 1) // - 1 because lookahead can be 4 but display has 3 rows (counting from 0)
                {
                    // Move bird up/down, depending on -/+ difference
                    if (rowDifference > 0)
                    {
                        currentBirdRow += 1;
                    }
                    else if (rowDifference < 0)
                    {
                        currentBirdRow -= 1;
                    }
                    break; // We took action? Don't make another movement in this update call
                }
            }
        }

        lcd.setCursor(2, currentBirdRow);
        lcd.write((byte) 3);


        moveOffset++;

        // Handle special case of showuntil = 0: Detect string end and force-Call nextPage()
        if (Config::showuntil[currentPage] == 0 && moveOffset == strlen(thisAnimation) - 1 ) nextPage();
    }
}
