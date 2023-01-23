/*
 * File: debug.cpp
 * Project: nodemcu-clock
 * Created Date: 14.01.2023 12:36:08
 * Author: 3urobeat
 * 
 * Last Modified: 23.01.2023 10:17:13
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


// Include debug() function if debug mode is enabled
#ifdef CLOCK_DEBUG

#include "helpers.h"

#ifdef ESP8266
    #include "cont.h"    // Needed to access CONT_STACKSIZE definition
#elif ESP32
    #include "sdkconfig.h"
    #define CONT_STACKSIZE CONFIG_MAIN_TASK_STACK_SIZE // Alias
#endif


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
        #ifdef ESP8266
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
        #elif ESP32
            Serial.print(F("Free Memory changed! - Stack: "));
            Serial.print(freeStack);
            Serial.print(F(" bytes (Diff: "));
            Serial.print(((int) freeStack) - ((int) lastFreeStack));
            Serial.print(F(" bytes, Min: "));
            Serial.print(heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT));
            Serial.print(F(" bytes) | Heap: "));
            Serial.print(freeHeap);
            Serial.print(F(" bytes (Diff: "));
            Serial.print(((int) freeHeap) - ((int) lastFreeHeap));
            Serial.println(F(" bytes)"));
        #endif

        // Refresh values
        lastFreeStack = freeStack;
        lastFreeHeap  = freeHeap;
    }
}

#endif