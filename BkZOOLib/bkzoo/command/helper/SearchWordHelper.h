/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_HELPER_SEARCH_WORD_HELPER_H
#define BKZ_COMMAND_HELPER_SEARCH_WORD_HELPER_H

#include <string>
#include <vector>
#include <map>

namespace bkzoo
{
    namespace command
    {
        namespace helper
        {

            typedef struct _PostParam
            {
                std::wstring actionUrl;
                std::map<std::wstring, std::wstring> params;
            } PostParam;

            // method get 用のURLからpost用のパラメータを生成する
            PostParam getPostParam(const std::wstring& originalUrl);

        }
    }
}

#endif // BKZ_COMMAND_HELPER_SEARCH_WORD_HELPER_H
