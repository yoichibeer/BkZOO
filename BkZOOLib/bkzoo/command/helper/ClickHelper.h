/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_HELPER_CLICK_HELPER_H
#define BKZ_COMMAND_HELPER_CLICK_HELPER_H

#include <Windows.h>

#include <string>

namespace bkzoo
{
    namespace command
    {
        namespace helper
        {

            // Click時の共通処理
            class ClickHelper final
            {
            public:
                // 設定ファイルでregexが設定されている検索サイトのうち対応しているサイトを一つだけ検索
                static bool executeRegexpSite(HWND hWnd, const std::wstring& string);
            };

        }
    }
}

#endif // BKZ_COMMAND_HELPER_CLICK_HELPER_H
