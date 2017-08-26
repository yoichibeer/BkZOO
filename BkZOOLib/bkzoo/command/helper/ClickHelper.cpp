/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "ClickHelper.h"

#include "command/SearchWord.h"
#include "bkzoo_config.h"

#include <string>
#include <regex>

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {
        namespace helper
        {

            bool ClickHelper::executeRegexpSite(HWND hWnd, const std::wstring& selectedString)
            {
                const auto& sites = Config::instance().sites();
                for (const auto& site : sites)
                {
                    // 有効でなければスキップ
                    if (!site.enabled_bool())
                        continue;

                    // クリックできない設定ならスキップ
                    if (!site.clickable_bool())
                        continue;

                    // 正規表現でヒットすれば検索実行
                    const std::wregex regex(site.regex());
                    if (std::regex_match(selectedString, regex))
                    {
                        const SearchWordCommand::Param param{ hWnd, selectedString, site.regex(), site.url(), site.encode_int(), site.method_enum(), url::urlencode };
                        SearchWordCommand searchWordCommand(param);
                        return searchWordCommand.execute();
                    }
                }

                return false;
            }

        }
    }
}
