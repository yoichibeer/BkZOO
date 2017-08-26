/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_CONFIG_CONFIGSERVICE_H
#define BKZ_CONFIG_CONFIGSERVICE_H

#include <Windows.h>

#include "defs.h"

namespace bkzoo
{
    namespace config
    {

        class ConfigService final
        {
        public:
            static bool initialize(HMODULE hModule);

        private:
            ConfigService() = delete;
            ~ConfigService() = delete;

            DISALLOW_COPY_AND_ASSIGN(ConfigService);
        };

    }
}

#endif // BKZ_CONFIG_CONFIGSERVICE_H
