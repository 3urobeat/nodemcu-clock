/*
 * File: stringUtils.cpp
 * Project: nodemcu-clock
 * Created Date: 23.12.2021 15:58:05
 * Author: 3urobeat
 * 
 * Last Modified: 15.01.2023 22:46:23
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
 */
void strrpl(char *src, const char *oldchars, const char *newchars, uint16_t maxlen) //Credit: https://forum.arduino.cc/t/replace-and-remove-char-arrays/485806/4
{
    char *p = strstr(src, oldchars);
    char buf[maxlen + 1];
    
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
 * Better concat function
 */
char *mystrcat(char *dest, const char *src) //Credit: https://stackoverflow.com/a/21881314
{
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return --dest;
}