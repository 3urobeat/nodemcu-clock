/*
 * File: setupWebPage.cpp
 * Project: nodemcu-clock
 * Created Date: 24.12.2022 19:02:04
 * Author: 3urobeat
 * 
 * Last Modified: 24.12.2022 19:43:59
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include "setupMode.h"

// TODO: Do this differently
const char webpage[] = "<html><head><title>nodemcu-clock Setup Page</title></head><body><h1>nodemcu-clock Setup Page</h1><p>Hello World</p></body></html>";

// Serves the page when user visits webserver
void setupModeWebPage()
{
    webserver.send(200, "text/html", webpage);
}