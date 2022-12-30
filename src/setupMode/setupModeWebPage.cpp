/*
 * File: setupWebPage.cpp
 * Project: nodemcu-clock
 * Created Date: 24.12.2022 19:02:04
 * Author: 3urobeat
 * 
 * Last Modified: 30.12.2022 21:54:22
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "setupMode.h"


// Processor function to dynamically replace placeholder variables in webpage
String processor(const String& var) // I need to use String here because the library forces me to... https://github.com/me-no-dev/ESPAsyncWebServer#send-large-webpage-from-progmem-containing-templates-and-extra-headers
{
    
    return String();
}


// Serves the page when user visits webserver
void setupModeWebPage(AsyncWebServerRequest *request)
{
    request->send_P(200, "text/html", webpage, processor); // webpage is defined in header file
}