/*
 * File: debug.cpp
 * Project: nodemcu-clock
 * Created Date: 14.01.2023 12:36:08
 * Author: 3urobeat
 * 
 * Last Modified: 14.01.2023 12:37:01
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "helpers.h"


/**
 * Helper function that logs available memory if values changed since last call. Config::DEBUG must be enabled!
 * @param str Optional flash string that will be logged before free mem message
 */
void debugMemory(const __FlashStringHelper *str)
{
    if (!Config::DEBUG) return;           // Ignore call if DEBUG is disabled
    if (str != NULL) Serial.println(str); // Log str param if one was passed

    // Store last measurements in static vars (they keep their values between function calls)
    static uint32_t lastFreeContStack;
    static uint32_t lastFreeHeap;
    
    // Get new measurements
    uint32_t freeContStack = ESP.getFreeContStack();
    uint32_t freeHeap      = ESP.getFreeHeap();

    // Send new measurements if they changed and update static vars
    if (freeContStack != lastFreeContStack || freeHeap != lastFreeHeap) {
        Serial.print(F("Memory Usage changed! - Stack: "));
        Serial.print(ESP.getFreeContStack());
        Serial.print(F(" bytes (Diff: "));
        Serial.print(((int) freeContStack) - ((int) lastFreeContStack));
        Serial.print(F(" bytes) | Heap: "));
        Serial.print(ESP.getFreeHeap());
        Serial.print(F(" bytes (Diff: "));
        Serial.print(((int) freeHeap) - ((int) lastFreeHeap));
        Serial.print(F(" bytes) | Heap Fragmentation: "));
        Serial.print(ESP.getHeapFragmentation());
        Serial.println(F("%"));

        // Refresh values
        lastFreeContStack = freeContStack;
        lastFreeHeap      = freeHeap;
    }
}