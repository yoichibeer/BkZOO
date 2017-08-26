/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */
#ifndef BKZ_SETTING_BACKGROUNDCOLOR_H
#define BKZ_SETTING_BACKGROUNDCOLOR_H

#include "defs.h"

#include <Windows.h>

namespace bkzoo
{
    namespace setting
    {

        class BackgroundColor final
        {
        public:
            BackgroundColor();
            ~BackgroundColor();

            HBRUSH hbrush() const;

        private:
            HBRUSH hbrush_;

        private:
            DISALLOW_COPY_AND_ASSIGN(BackgroundColor);
        };


    }
}

#endif // BKZ_SETTING_BACKGROUNDCOLOR_H
