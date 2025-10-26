/*
 * File: pages.h
 * Project: nodemcu-clock
 * Created Date: 2021-09-01 15:17:00
 * Author: 3urobeat
 *
 * Last Modified: 2025-10-26 20:11:17
 * Modified By: 3urobeat
 *
 * Copyright (c) 2021 - 2025 3urobeat <https://github.com/3urobeat>
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include "helpers/helpers.h" // Include helpers header which also includes main header
#include "pagesJsonParser.h"


namespace clockPage {
    extern const char *title;
    extern const int  updateInterval;
    extern const bool hideMiniClock;

    void setup();
    void update();
}

namespace weatherPage {
    extern const char *title;
    extern const int  updateInterval;
    extern const bool hideMiniClock;

    void setup();
    void update();
}

namespace newsPage {
    extern const char *title;
    extern const int  updateInterval;
    extern const bool hideMiniClock;

    void setup();
    void update();
}

namespace dinoPage {
    extern const char *title;
    extern const int  updateInterval;
    extern const bool hideMiniClock;

    void setup();
    void update();
}

namespace flappyPage {
    extern const char *title;
    extern const int  updateInterval;
    extern const bool hideMiniClock;

    void setup();
    void update();
}

namespace spotifyPage {
    extern const char *title;
    extern const int  updateInterval;
    extern const bool hideMiniClock;

    void setup();
    void update();
}
