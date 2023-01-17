/*
 * File: pages.h
 * Project: nodemcu-clock
 * Created Date: 01.09.2021 15:17:00
 * Author: 3urobeat
 * 
 * Last Modified: 17.01.2023 21:23:01
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2021 3urobeat <https://github.com/HerrEurobeat>
 * 
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work
 * Full license information available in the project LICENSE file.
 */


#pragma once

#include "helpers/helpers.h" // Include helpers header which also includes main header
#include "pagesJsonParser.h"


namespace clockPage {
    void setup();
    void update();
}

namespace weatherPage {
    void setup();
    void update();
}

namespace newsPage {
    void setup();
    void update();
}

namespace spotifyPage {
    void setup();
    void update();
}