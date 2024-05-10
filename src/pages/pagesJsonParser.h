/*
 * File: pagesJsonParser.h
 * Project: nodemcu-clock
 * Created Date: 2023-01-12 12:40:54
 * Author: 3urobeat
 *
 * Last Modified: 2024-05-10 11:14:45
 * Modified By: 3urobeat
 *
 * Copyright (c) 2023 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


// This file includes virtual handler classes for all JSON requests made from page functions

#pragma once


// Json parser class for openweathermap.com request
class WeatherJsonHandler final : public JsonHandler
{
    private:
        char     *_cond;
        uint8_t   _condSize;
        char     *_temp;
        uint32_t *_sunrise;
        uint32_t *_sunset;

    public:
        // Constructor that takes pointers to char arrays where the result should go to
        WeatherJsonHandler(char *cond, uint8_t condSize, char *temp, uint32_t *sunrise, uint32_t *sunset)
        {
            _cond     = cond;
            _condSize = condSize;
            _temp     = temp;
            _sunrise  = sunrise;
            _sunset   = sunset;
        }

        // Handles retrieving data from json stream
        void value(ElementPath path, ElementValue value)
        {
            // Get the current key
            const char *key = path.getKey();

            // Check which key we are dealing with and copy value into correct variable
            if (strcmp(key, "main") == 0) strncpy(_cond, value.getString(), _condSize - 1);
            else if (strcmp(key, "temp") == 0) itoa((int8_t) round(value.getFloat() - 273.15), _temp, 10); // Kelvin -> Celsius, round, cast to int and convert to char arr
            else if (strcmp(key, "sunrise") == 0) *_sunrise = value.getInt();
            else if (strcmp(key, "sunset") == 0) *_sunset = value.getInt();
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

            #ifdef CLOCK_DEBUG
                Serial.print("News: ");
                Serial.print(key);
                Serial.print(": ");
                Serial.println(value.getString());
            #endif

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
                strrpl(_pubAtCache + (_pubAtCacheSize * _currentIndex), "hh", lcd.toFixedLengthNumber(buf, hour(inLocalSeconds)  , 2), _pubAtCacheSize - 1);
                strrpl(_pubAtCache + (_pubAtCacheSize * _currentIndex), "mm", lcd.toFixedLengthNumber(buf, minute(inLocalSeconds), 2), _pubAtCacheSize - 1);

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


// Json parser class for spotify page refreshCurrentPlayback request
class SpotifyRefreshPlaybackJsonHandler final : public JsonHandler
{
    private:
        char     *_title;
        uint8_t   _titleSize;
        char     *_artist;
        uint8_t   _artistSize;
        uint32_t *_progressTimestamp;
        uint32_t *_songLength;
        bool     *_currentlyPlaying;

    public:
        // Constructor that takes pointers to char arrays where the result should go to
        SpotifyRefreshPlaybackJsonHandler(char *title, uint8_t titleSize, char *artist, uint8_t artistSize, uint32_t *progressTimestamp, uint32_t *songLength, bool *currentlyPlaying)
        {
            _title             = title;
            _titleSize         = titleSize;
            _artist            = artist;
            _artistSize        = artistSize;
            _progressTimestamp = progressTimestamp;
            _songLength        = songLength;
            _currentlyPlaying  = currentlyPlaying;
        }

        // Handles retrieving data from json stream
        void value(ElementPath path, ElementValue value)
        {
            // Get the current path (there are duplicate keys so we need the full path here)
            char _fullPath[128 + 32] = ""; // I like the power of 2, thank you Fabi for helping me to not write 160
            path.toString(_fullPath);

            if (strcmp(_fullPath, "progress_ms") == 0) {
                *_progressTimestamp = value.getInt();

            } else if (strcmp(_fullPath, "item.artists[0].name") == 0) {
                uint8_t len = strlen(value.getString());

                // Add two spaces to the front and back if >maxcol to make movingPrint() reset not as abrupt
                if (len > Config::maxcol) {
                    strcpy(_artist, "  ");
                    strncat(_artist, value.getString(), _artistSize - 5);
                    strcat(_artist, "  ");
                } else {
                    strncpy(_artist, value.getString(), _artistSize - 1); // There could be more artists, let's ignore that for now
                }
            } else if (strcmp(_fullPath, "item.duration_ms") == 0) {
                *_songLength = value.getInt();

            } else if (strcmp(_fullPath, "item.name") == 0) {
                uint8_t len = strlen(value.getString());

                // Add two spaces to the front and back if >maxcol to make movingPrint() reset not as abrupt
                if (len > Config::maxcol) {
                    strcpy(_title, "  ");
                    strncat(_title, value.getString(), _titleSize - 5);
                    strcat(_title, "  ");
                } else {
                    strncpy(_title, value.getString(), _titleSize - 1);
                }
            } else if (strcmp(_fullPath, "is_playing") == 0) *_currentlyPlaying = value.getBool();
        }

        virtual ~SpotifyRefreshPlaybackJsonHandler() = default; // To fix warning delete-non-virtual-dtor

        // Functions we don't care about (sadly can't be removed)
        void endDocument() { }
        void startDocument() { }
        void startObject(ElementPath path) { }
        void endObject(ElementPath path) { }
        void startArray(ElementPath path) {}
        void endArray(ElementPath path) {}
        void whitespace(char c) {}
};


// Json parser class for spotify page accessToken request
class SpotifyAccessTokenJsonHandler final : public JsonHandler
{
    private:
        char     *_accessToken;
        uint16_t  _accessTokenSize;
        uint32_t *_expiresTimestamp;

    public:
        // Constructor that takes pointers to char arrays where the result should go to
        SpotifyAccessTokenJsonHandler(char *accessToken, uint16_t accessTokenSize, uint32_t *expiresTimestamp)
        {
            _accessToken      = accessToken;
            _accessTokenSize  = accessTokenSize;
            _expiresTimestamp = expiresTimestamp;
        }

        // Handles retrieving data from json stream
        void value(ElementPath path, ElementValue value)
        {
            // Get the current key
            const char *key = path.getKey();

            // Check which key we are dealing with and copy value into correct variable
            if (strcmp(key, "access_token") == 0) strncpy(_accessToken, value.getString(), _accessTokenSize - 1);
            else if (strcmp(key, "expires_in") == 0) *_expiresTimestamp += value.getInt() * 1000; // add expires_in from sec to ms to millis() already in timestamp var
            else if (strcmp(key, "refresh_token") == 0) prefs.putBytes("spotifyRefToken", value.getString(), 256); // Put it directly into flash as we only need it every hour
        }

        virtual ~SpotifyAccessTokenJsonHandler() = default; // To fix warning delete-non-virtual-dtor

        // Functions we don't care about (sadly can't be removed)
        void endDocument() { }
        void startDocument() { }
        void startObject(ElementPath path) { }
        void endObject(ElementPath path) { }
        void startArray(ElementPath path) {}
        void endArray(ElementPath path) {}
        void whitespace(char c) {}
};
