/*
 * File: helpersJsonParser.h
 * Project: nodemcu-clock
 * Created Date: 11.01.2023 15:52:17
 * Author: 3urobeat
 * 
 * Last Modified: 30.06.2023 09:47:10
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/3urobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
 */


// This file includes virtual handler classes for all JSON requests made from helper functions

#pragma once


// Json parser class for ip-api.com request
class GetLocationJsonHandler1 final : public JsonHandler
{
    public:
        // Handles retrieving data from json stream
        void value(ElementPath path, ElementValue value)
        {
            // Get the current key
            const char *key = path.getKey();

            // Check which key we are dealing with and copy value into correct variable
            if (strcmp(key, "countryCode") == 0) strncpy(country, value.getString(), sizeof(country) - 1);
            else if (strcmp(key, "city")   == 0) strncpy(city, value.getString(), sizeof(city) - 1);
            else if (strcmp(key, "lat")    == 0) dtostrf(value.getFloat(), 6, 4, Config::lat); // Convert double to char arr and store result into Config
            else if (strcmp(key, "lon")    == 0) dtostrf(value.getFloat(), 6, 4, Config::lon);
            else if (strcmp(key, "offset") == 0) timeoffset = value.getInt();
        }

        virtual ~GetLocationJsonHandler1() = default; // To fix warning delete-non-virtual-dtor

        // Functions we don't care about (sadly can't be removed)
        void endDocument() { }
        void startDocument() { }      
        void startObject(ElementPath path) { }
        void endObject(ElementPath path) { }
        void startArray(ElementPath path) {}
        void endArray(ElementPath path) {}
        void whitespace(char c) {}
};


// Json parser class for openweathermap.org request
class GetLocationJsonHandler2 final : public JsonHandler
{
    public:
        // Handles retrieving data from json stream
        void value(ElementPath path, ElementValue value)
        {
            // Get the current key
            const char *key = path.getKey();

            // Check which key we are dealing with and copy value into correct variable
            if      (strcmp(key, "name")     == 0) strncpy(city, value.getString(), sizeof(city) - 1);
            else if (strcmp(key, "country")  == 0) strncpy(country, value.getString(), sizeof(country) - 1);
            else if (strcmp(key, "timezone") == 0) timeoffset = value.getInt();
        }

        virtual ~GetLocationJsonHandler2() = default; // To fix warning delete-non-virtual-dtor

        // Functions we don't care about (sadly can't be removed)
        void endDocument() { }
        void startDocument() { }      
        void startObject(ElementPath path) { }
        void endObject(ElementPath path) { }
        void startArray(ElementPath path) {}
        void endArray(ElementPath path) {}
        void whitespace(char c) {}
};