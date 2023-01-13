/*
 * File: pagesJsonParser.h
 * Project: nodemcu-clock
 * Created Date: 12.01.2023 12:40:54
 * Author: 3urobeat
 * 
 * Last Modified: 13.01.2023 20:11:21
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file. 
 */


// This file includes virtual handler classes for all JSON requests made from page functions

#pragma once


// Json parser class for openweathermap.com request
class WeatherJsonHandler final : public JsonHandler
{
    private:
        char   *_cond;
        uint8_t _condSize;
        char   *_temp;
    
    public:
        // Constructor that takes pointers to char arrays where the result should go to
        WeatherJsonHandler(char *cond, uint8_t condSize, char *temp)
        {
            _cond     = cond;
            _condSize = condSize;
            _temp     = temp;
        }

        // Handles retrieving data from json stream
        void value(ElementPath path, ElementValue value)
        {
            // Get the current key
            const char *key = path.getKey();

            // Check which key we are dealing with and copy value into correct variable
            if (strcmp(key, "main") == 0) strncpy(_cond, value.getString(), _condSize);
                else if (strcmp(key, "temp") == 0) itoa((int8_t) round(value.getFloat() - 273.15), _temp, 10); // Kelvin -> Celsius, round, cast to int and convert to char arr
        }

        virtual ~WeatherJsonHandler() = default; // To fix warning delete-non-virtual-dtor

        // Functions we don't care about (sadly can't be removed)
        void endDocument() { }
        void startDocument() { }      
        void startObject(ElementPath path) { }
        void endObject(ElementPath path) { }
        void startArray(ElementPath path) {}
        void endArray(ElementPath path) {}
        void whitespace(char c) {}
};