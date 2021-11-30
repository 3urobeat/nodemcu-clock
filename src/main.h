/*
 * File: main.h
 * Project: nodemcu-clock
 * Created Date: 30.11.2021 17:25:23
 * Author: 3urobeat
 * 
 * Last Modified: 30.11.2021 21:11:49
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
 */


#include <NoiascaLiquidCrystal.h>
#include <NoiascaHW/lcd_PCF8574.h>

extern LiquidCrystal_PCF8574 lcd;
extern int maxcol;
extern int lcdCursorPos[2];
extern String lcdContent[4];