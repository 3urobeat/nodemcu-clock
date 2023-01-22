/*
 * File: stringUtils.cpp
 * Project: nodemcu-clock
 * Created Date: 23.12.2021 15:58:05
 * Author: 3urobeat
 * 
 * Last Modified: 22.01.2023 10:24:07
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#include "helpers.h"


/**
 * Replace function for char arrays
 * @param src Pointer to source char array
 * @param oldchars Old chars to find and replace with newchars
 * @param newchars News chars that should replace oldchars
 * @param maxlen The maxmimum length an excerpt from src might have, is used for allocating buffer
 */
void strrpl(char *src, const char *oldchars, const char *newchars, uint16_t maxlen) //Credit: https://forum.arduino.cc/t/replace-and-remove-char-arrays/485806/4
{
    char *p = strstr(src, oldchars);
    char buf[maxlen + 1] = "";       // Init with empty char arr to clear any gibberish
    
    do {
        if (p) {
            memset(buf, '\0', strlen(buf));

            if (src == p) {
                strcpy(buf, newchars);
                strcat(buf, p + strlen(oldchars));
            } else {
                strncpy(buf, src, strlen(src) - strlen(p));
                strcat(buf, newchars);
                strcat(buf, p + strlen(oldchars));
            }

            memset(src, '\0', strlen(src));
            strcpy(src, buf);
        }
    } while (p && (p = strstr(src, oldchars)));
}


/**
 * Better concat function which is faster than strcat for repeated concats to the same char arr
 * @param dest Pointer to destination char array
 * @param src Char array to concatenate to dest
 * @returns New pointer to the end of dest, pass this one to the next mystrcat call as dest
 */
char *mystrcat(char *dest, const char *src) // Credit: https://stackoverflow.com/a/21881314
{
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return --dest;
}