/*
 * File: debug.cpp
 * Project: nodemcu-clock
 * Created Date: 14.01.2023 12:36:08
 * Author: 3urobeat
 * 
 * Last Modified: 17.01.2023 23:41:58
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


// Include debug() function if debug mode is enabled
#ifdef CLOCK_DEBUG

#include "helpers.h"
#include "cont.h"    // Needed to access CONT_STACKSIZE definition


/**
 * Helper function that logs available memory if values changed since last call. Config::DEBUG must be enabled!
 * @param str Optional flash string that will be logged before free mem message
 */
void debug(const __FlashStringHelper *str)
{
    if (str != NULL) Serial.println(str); // Log str param if one was passed

    // Store last measurements in static vars (they keep their values between function calls)
    static uint32_t lastFreeStack;
    static uint32_t lastFreeHeap;
    
    // Get new measurements
    byte stack_now; // Allocate something to get address of it

    uint32_t freeStack = CONT_STACKSIZE - ((uint32_t) stack_start - (uint32_t) &stack_now); // Subtract start addr from current addr to get current usage
    uint32_t freeHeap  = ESP.getFreeHeap();

    // Send new measurements if they changed and update static vars
    if (freeStack != lastFreeStack || freeHeap != lastFreeHeap) {
        Serial.print(F("Free Memory changed! - Stack: "));
        Serial.print(freeStack);
        Serial.print(F(" bytes (Diff: "));
        Serial.print(((int) freeStack) - ((int) lastFreeStack));
        Serial.print(F(" bytes, Min: "));
        Serial.print(ESP.getFreeContStack());
        Serial.print(F(" bytes) | Heap: "));
        Serial.print(freeHeap);
        Serial.print(F(" bytes (Diff: "));
        Serial.print(((int) freeHeap) - ((int) lastFreeHeap));
        Serial.print(F(" bytes) | Heap Fragm.: "));
        Serial.print(ESP.getHeapFragmentation());
        Serial.println(F("%"));

        // Refresh values
        lastFreeStack = freeStack;
        lastFreeHeap  = freeHeap;
    }
}

#endif