/*
 * File: fsInteraction.cpp
 * Project: nodemcu-clock
 * Created Date: 25.12.2022 15:51:11
 * Author: 3urobeat
 * 
 * Last Modified: 25.12.2022 19:46:32
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


// This file handles reading/writing from/to the ESP's internal filesystem. This is used to store and modify configuration data from setupMode.

#include "helpers.h"


/**
 * Reads file from internal filesystem and copies content to `dest`. If file doesn't exist `dest` will remain empty.
 * @param char Pointer to destination char array
 * @param filename Name of file to be read
 * @returns Pointer to dest
 */
char *readFromFS(char *dest, const char *filename)
{
    if (LittleFS.exists(filename)) {
        File thisFile = LittleFS.open(filename, "r");

        if (thisFile) { // Only continue if file was opened successfully
            size_t offset = 0; // Track progress to append char at the right place

            while (thisFile.available()) { // Append content to dest until finished reading
                *(dest + offset++) = (char) thisFile.read(); // Append char to dest and increment offset
            }

            *(dest + offset++) = '\0'; // Make sure last char is a null byte
        }

        thisFile.close(); // Close file when finished to clear mem
    }

    // Return pointer to dest
    return dest;
}


/**
 * Writes file to internal filesystem.
 * @param filename Name of the file to write to
 * @param content Content to write to file
 */
void writeToFS(const char *filename, char *content)
{
    File thisFile = LittleFS.open(filename, "w"); // Creates file if it doesn't exist

    if (thisFile) thisFile.print(content);
    
    thisFile.close();
}