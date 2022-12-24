/*
 * File: setupMode.h
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:51:00
 * Author: 3urobeat
 * 
 * Last Modified: 24.12.2022 19:30:12
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#pragma once

#include <ESP8266WebServer.h>
#include "helpers/helpers.h" // Include helpers header which also includes main header

extern bool setupModeEnabled;
extern ESP8266WebServer webserver;

bool setupSetupMode();
void hostSetupMode();
void handleSetupMode();

void setupModeWebPage();