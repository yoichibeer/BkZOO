/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_CONFIG_CONFIGFACTORY_H
#define BKZ_CONFIG_CONFIGFACTORY_H

#include "Config.h"

#include <Windows.h>

#include <vector>
#include <memory>

#include "defs.h"

namespace bkzoo
{
    namespace config
    {

        class ConfigFactory final
        {
        public:
            static std::unique_ptr<Config> ConfigFactory::create(
                const config::General& general,
                const SiteMap& presetSites,
                const SiteVector& sites
            );
        private:
            ConfigFactory() = default;
            ~ConfigFactory() = default;

            DISALLOW_COPY_AND_ASSIGN(ConfigFactory);
        };

    }
}

#endif // BKZ_CONFIG_CONFIGFACTORY_H
