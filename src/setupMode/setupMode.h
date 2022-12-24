/*
 * File: setupMode.h
 * Project: nodemcu-clock
 * Created Date: 23.12.2022 13:51:00
 * Author: 3urobeat
 * 
 * Last Modified: 24.12.2022 18:58:18
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2022 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#pragma once

#include "helpers/helpers.h" // Include helpers header which also includes main header


extern bool setupModeEnabled;

bool setupSetupMode();
void hostSetupMode();
void handleSetupMode();