/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_STRINGS_WIDE_CHAR_MULTI_BYTE_H
#define BKZ_STRINGS_WIDE_CHAR_MULTI_BYTE_H

#include <Windows.h>

#include <string>

enum : int
{
    CP51932 = 51932 // EUC-JP
};

namespace bkzoo
{
    namespace string
    {

        static std::string wideToMulti(const std::wstring& src, UINT codePage = CP_ACP, DWORD flags = 0);
        static std::wstring multiToWide(const std::string& src, UINT codePage = CP_ACP, DWORD flags = 0);

        template<typename Destination, typename Source>
        inline Destination wm_cast(const Source& src, UINT codePage = CP_ACP, DWORD flags = 0);

        template<>
        std::string wm_cast<std::string, std::wstring>(const std::wstring& src, UINT codePage, DWORD flags);

        template<>
        std::wstring wm_cast<std::wstring, std::string>(const std::string& src, UINT codePage, DWORD flags);

        template<>
        std::wstring wm_cast<std::wstring, std::wstring>(const std::wstring& src, UINT codePage, DWORD flags);

        template<>
        std::string wm_cast<std::string, std::string>(const std::string& src, UINT codePage, DWORD flags);

        template<typename Destination, typename Source>
        inline Destination wm_cast(const Source* src, UINT codePage = CP_ACP, DWORD flags = 0);

        template<>
        std::string wm_cast<std::string, wchar_t>(const wchar_t* src, UINT codePage, DWORD flags);

        template<>
        std::wstring wm_cast<std::wstring, char>(const char* src, UINT codePage, DWORD flags);

        template<>
        std::wstring wm_cast<std::wstring, wchar_t>(const wchar_t* src, UINT codePage, DWORD flags);

        template<>
        std::string wm_cast<std::string, char>(const char* src, UINT codePage, DWORD flags);

    }
}

#endif // BKZ_STRINGS_WIDE_CHAR_MULTI_BYTE_H
