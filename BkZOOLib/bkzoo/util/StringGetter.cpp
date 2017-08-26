/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "StringGetter.h"

#include "clipboard/Clipboard.h"

#include "bkzoo_string.h"

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
            std::wstring limitedLengthString = (inputString.length() <= limiterLength)
                ? inputString
                : inputString.substr(0, limiterLength - lastLength - 1).append(L"…").append(inputString.substr(inputString.length() - lastLength));
            return limitedLengthString;
        }


        std::wstring StringGetter::removedSpaceText(const std::wstring& inputString)
        {
            std::wstring removedSpaceString(inputString);

            // 選択文字列の最初と最後の空白（空白、改行、TAFなど）を削除
            string::StringUtils::trim(removedSpaceString);

            // 選択文字列の途中の改行を削除
            std::wregex replace_reg(L"[\\n\\r]+");
            removedSpaceString = std::regex_replace(removedSpaceString, replace_reg, L"");

            return removedSpaceString;
        }


        std::wstring StringGetter::selectedText(HWND hWnd)
        {

            std::wstring selectedString;

            // RevertClipboardは現在のクリップボードのデータをバックアップし、スコープ抜ける際に復元
            clipboard::RevertClipboard revertClipboard(hWnd);

            // Copyコマンド送って選択文字列取得
            ::SendMessage(hWnd, WM_COMMAND, 0xE122, 0);

            // Clipboardにコピーされた選択文字列取得
            clipboard::ScopedClipboard scopedClipboard(hWnd);
            selectedString = scopedClipboard.getClipboardData();
            scopedClipboard.close();

            return selectedString;
        }


        std::wstring StringGetter::clipboardText(HWND hWnd)
        {

            std::wstring clipboardText;

            if (!::IsClipboardFormatAvailable(CF_UNICODETEXT))
            {
                return clipboardText;
            }

            // Clipboardにコピーされた文字列取得
            clipboard::ScopedClipboard scopedClipboard(hWnd);
            clipboardText = scopedClipboard.getClipboardData();
            scopedClipboard.close();

            return clipboardText;
        }

    }
}
