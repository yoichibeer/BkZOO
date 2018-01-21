/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "StringGetter.h"

#include "clipboard/Clipboard.h"

#include "bkzoo_string.h"
#include "bkzoo_log.h"

#include <BeckyAPI.h>

#include <regex>

#include "debug/detect_memory_leak.h"

using namespace bkzoo;

namespace bkzoo
{
    namespace util
    {

        // 選択文字列を表示用に加工する（文字列の長さが長い場合に短縮形にする）.
        // param [in] limiterLength 選択文字列がこの値以下ならばそのまま表示（短縮しない）
        // param [in] lastLength 短縮形にする場合に文字列の最後から何文字を表示するかを指定
        std::wstring StringGetter::limitedLengthText(const std::wstring& inputString, size_t limiterLength, size_t lastLength)
        {
            if (lastLength >= limiterLength)
            {
                return std::wstring(L"");
            }

            // 選択文字列を表示用に加工（文字列の長さが長い場合に短縮形にする）
            const std::wstring limitedLengthString = (inputString.length() <= limiterLength)
                ? inputString
                : inputString.substr(0, limiterLength - lastLength - 1).append(L"…").append(inputString.substr(inputString.length() - lastLength));
            return limitedLengthString;
        }


        std::wstring StringGetter::removedSpaceText(const std::wstring& inputString)
        {
            std::wstring removedSpaceText(inputString);

            // 選択文字列の最初と最後の空白（空白、改行、TAFなど）を削除
            string::StringUtils::trim(removedSpaceText);

            // 選択文字列の途中の改行を削除
            const std::wregex replace_reg(L"[\\n\\r]+");
            removedSpaceText = std::regex_replace(removedSpaceText, replace_reg, L"");

            LOG_INFO << "removedSpaceText=" << removedSpaceText;
            return removedSpaceText;
        }


        std::wstring StringGetter::selectedText(HWND hWnd)
        {
            LOG_INFO << "hWnd=" << hWnd;

            // RevertClipboardは現在のクリップボードのデータをバックアップし、スコープ抜ける際に復元
            clipboard::RevertClipboard revertClipboard(hWnd);

            // Copyコマンド送って選択文字列取得
            const LRESULT ret = ::SendMessage(hWnd, WM_COMMAND, 0xE122, 0);
            if ((ret != S_OK) && (ret != S_FALSE))
                LOG_ERROR << "ret=" << ret << ", SendMessage failed.";

            // Clipboardにコピーされた選択文字列取得
            const clipboard::ScopedClipboard scopedClipboard(hWnd);
            const std::wstring selectedText = scopedClipboard.getClipboardData();

            LOG_INFO << "selectedText=" << selectedText;
            return selectedText;
        }


        std::wstring StringGetter::clipboardText(HWND hWnd)
        {
            if (!::IsClipboardFormatAvailable(CF_UNICODETEXT))
            {
                LOG_INFO << "IsClipboardFormatAvailable(IsClipboardFormatAvailable) return false";
                return std::wstring();
            }

            // Clipboardにコピーされた文字列取得
            const clipboard::ScopedClipboard scopedClipboard(hWnd);
            const std::wstring clipboardText = scopedClipboard.getClipboardData();

            LOG_INFO << "clipboardText=" << clipboardText;
            return clipboardText;
        }

    }
}
