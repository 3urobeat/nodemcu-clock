/*
 * File: main.h
 * Project: nodemcu-clock
 * Created Date: 30.11.2021 17:25:23
 * Author: 3urobeat
 * 
 * Last Modified: 26.10.2022 14:10:53
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


#include <NoiascaLiquidCrystal.h>
#include <NoiascaHW/lcd_PCF8574.h>
#include <lcdHelper.h>

extern lcdHelper<LiquidCrystal_PCF8574> lcd;
extern const int maxcol;