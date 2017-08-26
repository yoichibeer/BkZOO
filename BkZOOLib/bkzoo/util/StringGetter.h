/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_STRING_GETTER_H
#define BKZ_UTIL_STRING_GETTER_H

#include <Windows.h>

#include <string>

namespace bkzoo
{
    namespace util
    {

        class StringGetter final
        {
        public:
            static std::wstring limitedLengthText(const std::wstring& inputString,
                size_t limiterLength = 50 /* 50 is same as chrome */,
                size_t lastLength = 12 /* 12 = filename 8 + ext 4 */);

            static std::wstring removedSpaceText(const std::wstring& inputString);

            static std::wstring selectedText(HWND hWnd);

            static std::wstring clipboardText(HWND hWnd);
        };

    }
}

#endif // BKZ_UTIL_STRING_GETTER_H
