/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "bkzoo_tut.h"

#include <sstream>

namespace tut
{

    std::string M(int lineNum)
    {
        std::ostringstream message;
        message << "L." << lineNum;
        return message.str();
    }

}
