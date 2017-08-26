/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_GET_RESOURCE_STRING_H
#define BKZ_UTIL_GET_RESOURCE_STRING_H

#include <Windows.h>

#include <string>

namespace bkzoo
{
    namespace util
    {

        std::wstring getResourceStringW(UINT uID);
        //        std::string getResourceStringA(UINT uID);

    }
}

#endif // BKZ_UTIL_GET_RESOURCE_STRING_H
