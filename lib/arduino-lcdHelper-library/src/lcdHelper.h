/*
 * File: lcdHelper.h
 * Project: arduino-lcdHelper-library
 * Created Date: 26.08.2022 12:04:51
 * Author: 3urobeat
 * 
 * Last Modified: 21.11.2022 19:16:44
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */

// Version 1.0.0

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