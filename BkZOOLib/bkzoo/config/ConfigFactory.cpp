/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "ConfigFactory.h"

#include <algorithm>
#include <iterator>

namespace bkzoo
{
    namespace config
    {

        std::unique_ptr<Config> ConfigFactory::create(
            const config::General& general,
            const SiteMap& presetSites,
            const SiteVector& standardSites
        )
        {
            std::unique_ptr<Config> config(new Config());
            // std::unique_ptr<Config> config(std::make_unique<Config>()); // Factoryをfriendにしてるがこの行はunique_ptrなのでコンパイル通らない。

            // 新しいpreset sitesがないかチェックするため。
            SiteMap newPresetSites(presetSites);

            // configに登録するsitesを生成
            SiteList sites;
            for (const Site& site : standardSites)
            {
                if (!site.preset_bool())
                {
                    // custom site を追加
                    sites.push_back(site);
                    continue;
                }

                // preset site を追加
                const int presetid = site.presetid_int();
                auto it = presetSites.find(presetid);
                if (it == presetSites.end())
                    continue; // preset sitesが更新されて削除されていた場合にありうる。

                // enabled を反映してpreset siteを追加する
                Site presetSite(it->second);
                presetSite.setEnabled(site.enabled());
                sites.push_back(presetSite);

                // sitesに追加した分は削除する
                newPresetSites.erase(presetid);
            }

            // preset sitesが更新されて新規追加されていいた場合にありうる。
            for (const auto& r : newPresetSites)
            {
                sites.push_back(r.second);
            }

            // configを生成
            config->setGeneral(general);
            config->setSites(sites);

            return config;
        }

    }

}
