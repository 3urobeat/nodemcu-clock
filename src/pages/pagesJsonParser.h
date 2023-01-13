/*
 * File: pagesJsonParser.h
 * Project: nodemcu-clock
 * Created Date: 12.01.2023 12:40:54
 * Author: 3urobeat
 * 
 * Last Modified: 13.01.2023 23:46:11
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
            if (strcmp(key, "main") == 0) strncpy(_cond, value.getString(), _condSize - 1);
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


// Json parser class for newsapi.org request
class NewsJsonHandler final : public JsonHandler
{
    private:
        char    *_sourceCache;
        uint8_t  _sourceCacheSize;
        char    *_pubAtCache;
        uint8_t  _pubAtCacheSize;
        char    *_titleCache;
        uint16_t _titleCacheSize;
        uint8_t  _cacheSize;      // Thats the amount of different articles in cache

        uint8_t  _currentIndex = 0;   // Stores index of article we are at right now
    
    public:
        // Constructor that takes pointers to char arrays where the result should go to
        NewsJsonHandler(char *sourceCache, uint8_t sourceCacheSize, char *pubAtCache, uint8_t pubAtCacheSize, char *titleCache, uint16_t titleCacheSize, uint8_t cacheSize) // Big boy sheesh
        {
            _sourceCache     = sourceCache;
            _sourceCacheSize = sourceCacheSize;
            _pubAtCache      = pubAtCache;
            _pubAtCacheSize  = pubAtCacheSize;
            _titleCache      = titleCache;
            _titleCacheSize  = titleCacheSize;
            _cacheSize       = cacheSize;
        }

        // Handles retrieving data from json stream
        void value(ElementPath path, ElementValue value)
        {
            // Get the current key
            const char *key = path.getKey();

            if (_currentIndex >= _cacheSize) return; // This should never happen when the URL has the right amount of pages set but let's make sure

            // Quick pointer arithmetic to make passing 2D arrays easier. We just pass pointer to element 0 of array to function and offset correctly inside function wiht passed sizes to reach desired element
            if (strcmp(key, "name") == 0) {
                strncpy(_sourceCache + (_sourceCacheSize * _currentIndex), value.getString(), _sourceCacheSize - 1);
                
            } else if (strcmp(key, "title") == 0) {
                // First copy two spaces to the front so the title doesn't start as "abruptly" when being displayed later
                char *p = _titleCache + (_titleCacheSize * _currentIndex);
                
                strcpy(p, "  ");
                strncpy(p + 2, value.getString(), _titleCacheSize - 5); // Concat the actual title after the two spaces, limit by available space - 5 (2 spaces + 2 spaces + null byte)
                strcat(p, "  \0"); // Add two more spaces and a null byte behind the title

            } else if (strcmp(key, "publishedAt") == 0) {
                char buf[10] = ""; // Create smol buffer

                // Substring "2021-12-15T07:15:04Z" to "07:15" by doing src pointer + offset
                strncpy(buf, value.getString() + strcspn(value.getString(), "T") + 1, _pubAtCacheSize - 1);

                // Convert hours and minutes to seconds to add timeoffset
                char hours[3] = { buf[0], buf[1], '\0' };   // char 0 and 1 are digits from hour
                char minutes[3] = { buf[3], buf[4], '\0' }; // char 3 and 4 are digits from minutes

                uint32_t inLocalSeconds = (atoi(hours) * 3600) + (atoi(minutes) * 60) + timeoffset; // Calculate seconds and add timeoffset to get local publishing time // TODO: What happens if article is published at 23 o clock UTC?

                // Format time using miniClockFormat, copy into pubAtCache
                strncpy(_pubAtCache + (_pubAtCacheSize * _currentIndex), Config::miniClockFormat, _pubAtCacheSize - 1);
                
                // Replace format chars that are now in _titleCache
                strrpl(_pubAtCache + (_pubAtCacheSize * _currentIndex), "hh", lcd.toFixedLengthNumber(buf, hour(inLocalSeconds), 2));
                strrpl(_pubAtCache + (_pubAtCacheSize * _currentIndex), "mm", lcd.toFixedLengthNumber(buf, minute(inLocalSeconds), 2));
                
            } else if (strcmp(key, "content") == 0) {
                _currentIndex++; // content is the last transmitted key of an article, so lets increment our index
            }
        }

        virtual ~NewsJsonHandler() = default; // To fix warning delete-non-virtual-dtor

        // Functions we don't care about (sadly can't be removed)
        void endDocument() { }
        void startDocument() { }
        void startObject(ElementPath path) { }
        void endObject(ElementPath path) { }
        void startArray(ElementPath path) {}
        void endArray(ElementPath path) {}
        void whitespace(char c) {}
};