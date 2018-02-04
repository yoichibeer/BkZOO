/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "SearchWordHelper.h"

#include "bkzoo_string.h"

#include <sstream>
#include <regex>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {
        namespace helper
        {

            PostParam getPostParam(const std::wstring& originalUrl)
            {
                PostParam postParam;

                const std::wregex regex(L"^(http.*)\\?(.*)$");
                std::wsmatch results;
                if (!std::regex_search(originalUrl, results, regex))
                {
                    return postParam;
                }
                if (results.size() != 3)
                {
                    return postParam;
                }

                // actionUrl
                postParam.actionUrl = results[1];

                // params
                const std::vector<std::wstring> queries = StringUtils::split(static_cast<std::wstring>(results[2]), L'&');
                for (const std::wstring query : queries)
                {
                    const std::vector<std::wstring> keyValue = StringUtils::split(query, L'=');
                    if (keyValue.size() != 2)
                    {
                        continue;
                    }
                    postParam.params[keyValue[0]] = keyValue[1];
                }

                return postParam;
            }

        }
    }
}
