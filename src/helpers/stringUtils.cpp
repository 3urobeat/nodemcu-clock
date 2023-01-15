/*
 * File: stringUtils.cpp
 * Project: nodemcu-clock
 * Created Date: 23.12.2021 15:58:05
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 22:48:01
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
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