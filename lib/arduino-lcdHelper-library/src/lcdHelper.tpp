/*
 * File: lcdHelper.tpp
 * Project: arduino-lcdHelper-library
 * Created Date: 28.08.2022 22:55:04
 * Author: 3urobeat
 * 
 * Last Modified: 08.09.2022 20:08:33
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file. 
 */

// Template implementation file
// https://stackoverflow.com/a/495056


template <typename lcd>
void lcdHelper<lcd>::clearLine(uint8_t row) {

    // print _lcdCols amount of spaces to effectively clear the line
    char str[this->_lcdCols + 1]; // make space for null byte

    for (int i = 0; i < this->_lcdCols; i++) {
        str[i] = ' ';
    }

    str[this->_lcdCols] = '\0';

    this->setCursor(0, row);
    this->print(str);
    
}

template <typename lcd>
void lcdHelper<lcd>::centerPrint(const char *str, uint8_t row, bool callClearLine) {

    // clear the line first to avoid old characters corrupting the text when content is not the same
    if (callClearLine) this->clearLine(row);

    // Calculate column
    int offset = this->_lcdCols - this->utf8_strlen(str);
    if (offset < 0) offset = 0; //set offset to 0 if it would be negative

    this->setCursor(offset / 2, row); //center string
    this->print(str);

}

template <typename lcd>
void lcdHelper<lcd>::movingPrint(const char *str, uint8_t *moveOffset, uint8_t width) {

    // check if we actually have to move something
    if (strlen(str) > width) {
        if (*moveOffset + width > strlen(str)) *moveOffset = 0; // reset if string was fully displayed

        char temp[width + 1] = ""; // leave space for next char and null byte

        strncpy(temp, str + *moveOffset, width); // substring to current offset
        
        // Fix for Umlaute: Add more chars if at least one two byte long char is included to avoid message being too short on the display
        //strncat(temp, str + width + moveOffset, width - this->utf8_strlen(temp));
        // Using Umlaute is still a bit janky but this is definitely an improvement. I'm not sure right now what else I could do.

        // TODO: lcd.movingPrint("Größe Menschen mit größen Füßen bräuchen größe Schuhe", 1);

        // Print current string
        this->print(temp);

        // Increase offset
        (*moveOffset)++;

    } else {
        
        this->print(str);
    }

}

template <typename lcd>
void lcdHelper<lcd>::alignedPrint(const char *align, const char *str, uint8_t width) {
    
    // Workarounds to correctly display Umlaute
    size_t len = utf8_strlen(str); // length on the display
    size_t blen = strlen(str);     // actual length in mem

    char temp[width + (blen - len) + 1] = ""; // blen - len to correct deviation caused by 2 byte long chars (Umlaute)

    // check if we even have to do something
    if (len == width) {
        this->print(str); 
        return;
    }

    // check if string is too long, cut it and display it as is
    if (len > width) {
        strncpy(temp, str, width);
        this->print(temp);

    } else { //if shorter, align text as requested

        // switch case doesn't work with strings so here we go
        if (strcmp(align, "left") == 0) {
            strcpy(temp, str);
            memset(temp + blen, ' ', width - len); // fill remaining space with spaces and keep existing null byte at the end

        } else if (strcmp(align, "center") == 0) {
            int offset = (width - len) / 2; // calculate offset to the left

            memset(temp, ' ', offset); // offset str with spaces
            strcat(temp, str);         // put str into the middle
            memset(temp + offset + blen, ' ', width - offset - len); // fill remaining space with spaces
            
        } else if (strcmp(align, "right") == 0) {
            memset(temp, ' ', width - len); // offset string
            strcpy(temp + width - len, str);
        }

        this->print(temp);
    }

}