/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "GetResourceString.h"

#include "debug/detect_memory_leak.h"

extern HMODULE hModuleBkZOO;

namespace bkzoo
{
    namespace util
    {

        std::wstring getResourceStringW(UINT uID)
        {
            wchar_t buffer[_MAX_PATH] = { 0 };
            ::LoadStringW(::hModuleBkZOO, uID, buffer, _MAX_PATH);
            return std::wstring(buffer);
        }

        //std::string getResourceStringA(UINT uID)
        //{
        //    char buffer[_MAX_PATH] = { 0 };
        //    ::LoadStringA(::hModuleBkZOO, uID, buffer, _MAX_PATH);
        //    return std::string(buffer);
        //}

    }
}
