/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include <BeckyAPI.h>

#include "util/StringGetter.h"
#include "clipboard/Clipboard.h"

#include "for_debug/detect_memory_leak.h"

using namespace bkzoo::clipboard;
using namespace bkzoo::util;

CBeckyAPI beckyApi;

namespace tut
{

    // Data used by each test
    struct StringGetter_data
    {
        virtual ~StringGetter_data()
        {
        }
    };

    // Test group registration
    typedef test_group<StringGetter_data> factory;
    typedef factory::object object;

}

namespace
{

    tut::factory tf("StringGetter");

}

namespace tut
{

    // removedSpaceText
    template<>
    template<>
    void object::test<1>()
    {
        const std::wstring removedSpaceString = StringGetter::removedSpaceText(L" \t\n\rabc\ndef\r \tg \thi \t\r\n");

        ensure_equals(M(__LINE__), removedSpaceString, L"abcdef \tg \thi");
    }


    // clipboardText
    template<>
    template<>
    void object::test<2>()
    {
        {
            ScopedClipboard scopedClipboard(nullptr);
            ::EmptyClipboard();
            std::wstring clipboardData(L" \t\n\rabc\ndef\r \tg \thi \t\r\n");
            const HGLOBAL hText = ::GlobalAlloc(GMEM_MOVEABLE, clipboardData.length() * sizeof(wchar_t) + 2);
            wchar_t* pText = static_cast<wchar_t*>(::GlobalLock(hText));
            ::lstrcpy(pText, clipboardData.c_str());
            ::GlobalUnlock(hText);
            ::SetClipboardData(CF_UNICODETEXT, hText);
        }

        std::wstring clipboardString = StringGetter::clipboardText(nullptr);
        ensure_equals(M(__LINE__), clipboardString, L" \t\n\rabc\ndef\r \tg \thi \t\r\n");
    }


    // limitedLengthText
    template<>
    template<>
    void object::test<3>()
    {
        std::wstring limitedLengthString = StringGetter::limitedLengthText(
            L"01234567890123456789012345678901234567890123456789");
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(L"01234567890123456789012345678901234567890123456789"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"012345678901234567890123456789012345678901234567890");
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(L"0123456789012345678901234567890123456…901234567890"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９");
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(
                L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９A");
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(
                L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６…９０１２３４５６７８９A"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８AB");
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(
                L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６…９０１２３４５６７８AB"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９Ａ");
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(
                L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６…９０１２３４５６７８９Ａ"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９ＡＢＣＤＥＦＧＨＩＪ");
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(
                L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６…８９ＡＢＣＤＥＦＧＨＩＪ"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９ＡＢＣＤＥＦＧＨＩＪ",
            20U,
            3U);
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(L"０１２３４５６７８９０１２３４５…ＨＩＪ"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９ＡＢＣＤＥＦＧＨＩＪ",
            5U,
            4U);
        ensure_equals(M(__LINE__), limitedLengthString, std::wstring(L"…ＧＨＩＪ"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９ＡＢＣＤＥＦＧＨＩＪ",
            5U,
            0U);
        ensure_equals(M(__LINE__), limitedLengthString, std::wstring(L"０１２３…"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９ＡＢＣＤＥＦＧＨＩＪ",
            100U,
            50U);
        ensure_equals(M(__LINE__), limitedLengthString,
            std::wstring(
                L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９ＡＢＣＤＥＦＧＨＩＪ"));
        limitedLengthString = StringGetter::limitedLengthText(
            L"０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９ＡＢＣＤＥＦＧＨＩＪ",
            10U,
            10U);
        ensure_equals(M(__LINE__), limitedLengthString, std::wstring(L""));
    }

}
