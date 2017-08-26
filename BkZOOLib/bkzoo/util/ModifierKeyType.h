/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_MODIFIER_KEY_H
#define BKZ_UTIL_MODIFIER_KEY_H

namespace bkzoo
{
    namespace util
    {

        enum ModifierKeyType : unsigned long
        {
            UNDEFINED = 0,
            Shift = 0x0001,
            Ctrl = 0x0002,
            Alt = 0x0004,
        };

    }
}

using namespace bkzoo::util;

#endif // BKZ_UTIL_MODIFIER_KEY_H
