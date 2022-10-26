/*
 * File: stringUtils.tpp
 * Project: helpers
 * Created Date: 28.08.2022 23:42:19
 * Author: 3urobeat
 * 
 * Last Modified: 08.09.2022 13:13:47
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */


template <typename lcd>
size_t lcdHelper<lcd>::utf8_strlen(const char *str) {
    
    int len = 0;
    while (*str) len += (*str++ & 0xc0) != 0x80;
    return len;

}


template <typename lcd>
char* lcdHelper<lcd>::toFixedLengthNumber(char *dest, int num, uint8_t len) {

    char numStr[len + 1] = ""; // no real way to check how long num will be so let's just use len as it will always be >=

    itoa(num, numStr, 10); // convert int and save into numStr

    memset(dest, '0', len - strlen(numStr)); // set all chars except the ones used by numStr to 0 (not \0 kek)
    strcpy(dest + (len - strlen(numStr)), numStr); // finally add the number itself to the end (use strcpy instead of strcat to make sure numStr ends up at the correct place)

    return dest; // return pointer to dest again to make using the function inside another call easier

}