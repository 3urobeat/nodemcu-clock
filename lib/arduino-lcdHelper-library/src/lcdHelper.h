/*
 * File: lcdHelper.h
 * Project: arduino-lcdHelper-library
 * Created Date: 2022-08-26 12:04:51
 * Author: 3urobeat
 *
 * Last Modified: 2025-08-30 15:44:04
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */

// Version 1.2.0

#ifndef LCDHELPER_H
#define LCDHELPER_H


template <typename lcd>
class lcdHelper : public lcd { //use template as base to inherit from
public:

    /**
     * Constructor
     * @param addr The address of your display
     * @param cols The amount of columns your display has
     * @param rows The amount of rows your display has
     */
    lcdHelper(uint8_t addr, uint8_t cols, uint8_t rows) : lcd(addr, cols, rows) {
        _lcdCols = cols;
        _lcdRows = rows;
    };

    /**
     * Clears a specific line on your display
     * @param row The row to clear (counted from 0)
     */
    void clearLine(uint8_t row);

    /**
     * Gets the offset from the beginning of a row to display a char array centered. If the char array is longer than the display width, 0 will be returned.
     * @param str The char array to get the offset for
     * @return Column offset from the beginning of a row
     */
    int calculateCenterOffset(const char *str);

    /**
     * Print a char array centered in a row on your display.
     * @param str The char array to print
     * @param row The row to print the char array in
     * @param callClearLine Optional: Set to true if line should be cleared before printing
     */
    void centerPrint(const char *str, uint8_t row, bool callClearLine = false);

    /**
     * Prints a char array that will be moved across a row by one char each time the function is called
     * @param str The char array to print
     * @param moveOffset Pointer to int tracking offset
     * @param width Width of the space on screen the char array will be moved across
     */
    void movingPrint(const char *str, uint8_t *moveOffset, uint8_t width);

    /**
     * Prints a char array that will fade in left to right, or right to left, from the current cursor position
     * @param str The char array to print
     * @param fadeInDelay Optional: Set a different delay between characters than the default of 50ms
     * @param rightToLeft Optional: Set to true to print from right to left instead of left to right
     * @param currentCol Semi-Optional: If rightToLeft is true, you must provide the current cursor column here, as the function has to control the cursor now
     * @param currentRow Semi-Optional: If rightToLeft is true, you must provide the current cursor row here, as the function has to control the cursor now
     */
    void fadeInPrint(const char *str, uint8_t fadeInDelay = 50, bool rightToLeft = false, uint8_t currentCol = 0, uint8_t currentRow = 0);

    /**
     * Print an animation frame by frame each time the function is called
     * @param animationArray Pointer to an array containing char arrays for each animation frame
     * @param animationSize Amount of frames your animation has, counting from 1
     * @param animationFrame Pointer to int tracking animation progress
     * @param col The column to print the animation at
     * @param row The row to print the animation in
     */
    void animationPrint(const char **animationArray, uint8_t animationSize, uint8_t *animationFrame, uint8_t col, uint8_t row);

    // Provide a few default animation (see chars that are referenced as hex here: https://werner.rothschopf.net/2020/lcd_charset_a00.gif)
    struct {
        const char *loading[8]  = { "|", "/", "-", "\x60", "|", "/", "-", "\x60" };                                                       // animationSize: 8
        const char *waiting[5]  = { "    ", ".   ", "..  ", "... ", "...." };                                                             // animationSize: 5
        const char *bounce[10]  = { "=     ", " =    ", "  =   ", "   =  ", "    = ", "     =", "    = ", "   =  ", "  =   ", " =    " }; // animationSize: 10
        const char *progress[6] = { "     ", "\xFF    ", "\xFF\xFF   ", "\xFF\xFF\xFF  ", "\xFF\xFF\xFF\xFF ", "\xFF\xFF\xFF\xFF\xFF" };  // animationSize: 6
        const char *arrows[5]   = { "    ", ">   ", ">>  ", ">>> ", ">>>>" };                                                             // animationSize: 5
        const char *bouncearrow[10] = { ">    ", " >   ", "  >  ", "   > ", "    >", "    <", "   < ", "  <  ", " <   ", "<    " };       // animationSize: 10
    } animations;

    /**
     * Print a char array aligned left, center or right to a fixed width.
     * @param align "left", "center" or "right"
     * @param str The char array to print
     * @param width The fixed width of the resulting char array, which str will be aligned to
     */
    void alignedPrint(const char *align, const char *str, uint8_t width);

    /**
     * Prints a char array to the display with a limited length (UTF-8 aware) without making a copy.
     * @param str The char array to print
     * @param length The length to limit str to
     */
    void limitedPrint(const char *str, uint8_t length);

    /**
     * Custom strlen function to correctly count chars that are two bytes long (like ä ö or ü)
     * @param str The char array to get the length of
     * @return Length of str
     */
    size_t utf8_strlen(const char *str);

    /**
     * Converts num to char array and precedes it with zeroes to match length.
     * Make sure buf is at least len bytes long!
     * @param dest Destination char array
     * @param num The number to convert
     * @param len The length the resulting char array will have. Zeroes will be added infront of num until it matches this length
     */
    char* toFixedLengthNumber(char *dest, int num, uint8_t len);

private:

    uint8_t _lcdCols;
    uint8_t _lcdRows;

};


// Include template implementation file
#include "lcdHelper.tpp"

#endif
