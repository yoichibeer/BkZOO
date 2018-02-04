/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "WideCharMultiByte.h"

#include "mlangApi.h"

#include <BeckyAPI.h>

#include <vector>

#include "for_debug/detect_memory_leak.h"

::bkzoo::string::MlangApi mlang;

namespace bkzoo
{
    namespace string
    {

        std::string wideToMulti(const std::wstring& src, UINT codePage, DWORD flags)
        {
            int length = ::WideCharToMultiByte(codePage, flags, src.c_str(),
                static_cast<int>(src.size()), nullptr, 0, nullptr, nullptr);
            if (length == 0)
            {
                return "";
            }

            std::vector<char> dst(length);
            length = ::WideCharToMultiByte(codePage, flags, src.c_str(),
                static_cast<int>(src.size()), &dst[0], static_cast<int>(dst.size()), nullptr, nullptr);
            assert(length != 0);
            return std::string(dst.begin(), dst.begin() + length);
        }

        std::string unicodeToEUC(const std::wstring& src, UINT codePage)
        {
            if (!::mlang.isInitialized())
            {
                return "";
            }
            DWORD mode = 0;
            int dstLength = 0;
            HRESULT error = ::mlang.ConvertINetUnicodeToMultiByte(&mode, codePage,
                src.c_str(), nullptr, nullptr, &dstLength);
            if (error != S_OK)
            {
                return "";
            }
            if (dstLength == 0)
            {
                return "";
            }

            std::vector<char> dst(dstLength);
            error = ::mlang.ConvertINetUnicodeToMultiByte(&mode, codePage,
                src.c_str(), nullptr, &dst[0], &dstLength);
            if (error != S_OK)
            {
                return "";
            }
            return std::string(dst.begin(), dst.begin() + dstLength);
        }

#if 0
        //動作せず。Becky!側のEUC_JP()で落ちる。2013/03/02
        std::string unicodeToEUC_BK(const std::wstring& src, UINT /* codePage */)
        {
            std::string shiftJisStr = wideToMulti(src);
            LPSTR pEucStr = ::beckyApi.EUC_JP(shiftJisStr.c_str(), TRUE);
            std::string eucStr(pEucStr);
            ::beckyApi.Free(pEucStr);
            return eucStr;
        }
#endif

        std::wstring multiToWide(const std::string& src, UINT codePage, DWORD flags)
        {
            int length = ::MultiByteToWideChar(codePage, flags, src.c_str(),
                static_cast<int>(src.size()), nullptr, 0);
            if (length == 0)
            {
                return L"";
            }

            std::vector<wchar_t> dst(length);
            length = ::MultiByteToWideChar(codePage, flags, src.c_str(),
                static_cast<int>(src.size()), &dst[0], static_cast<int>(dst.size()));
            assert(length != 0);
            return std::wstring(dst.begin(), dst.begin() + length);
        }

        std::wstring eucToUNICODE(const std::string& src, UINT codePage)
        {
            if (!::mlang.isInitialized())
            {
                return L"";
            }
            DWORD mode = 0;
            int dstLength = 0;
            HRESULT error = ::mlang.ConvertINetMultiByteToUnicode(&mode, codePage,
                src.c_str(), nullptr, nullptr, &dstLength);
            if (error != S_OK)
            {
                return L"";
            }
            if (dstLength == 0)
            {
                return L"";
            }

            std::vector<wchar_t> dst(dstLength);
            error = ::mlang.ConvertINetMultiByteToUnicode(&mode, codePage,
                src.c_str(), nullptr, &dst[0], &dstLength);
            if (error != S_OK)
            {
                return L"";
            }
            return std::wstring(dst.begin(), dst.begin() + dstLength);
        }

#if 0
        //動作せず。Becky!側のEUC_JP()で落ちる。2013/03/02
        std::wstring eucToUNICODE_BK(const std::string& src, UINT codePage)
        {
            LPSTR pShiftJIS_Str = ::beckyApi.EUC_JP(src.c_str(), FALSE);
            std::string shiftJisStr(pShiftJIS_Str);
            ::beckyApi.Free(pShiftJIS_Str);
            std::wstring unicodeStr = multiToWide(shiftJisStr, codePage);
            return unicodeStr;
        }
#endif

        template<>
        std::string wm_cast<std::string, std::wstring>(const std::wstring& src, UINT codePage, DWORD flags)
        {
            if (codePage == CP51932)
            {
                return unicodeToEUC(src, codePage);
            }
            //if (CP_EUC_BECKY == codePage){
            //動作せず。Becky!側のEUC_JP()で落ちる。2013/03/02
            //    return unicodeToEUC_BK(src, codePage);
            //}
            return wideToMulti(src, codePage, flags);
        }

        template<>
        std::wstring wm_cast<std::wstring, std::string>(const std::string& src, UINT codePage, DWORD flags)
        {
            if (codePage == CP51932)
            {
                return eucToUNICODE(src, codePage);
            }
            //if (codePage == CP_EUC_BECKY){
            //動作せず。Becky!側のEUC_JP()で落ちる。2013/03/02
            //    codePage = CP_ACP;
            //    return eucToUNICODE_BK(src, codePage);
            //}
            return multiToWide(src, codePage, flags);
        }

        template<>
        std::wstring wm_cast<std::wstring, std::wstring>(const std::wstring& src, UINT /*codePage*/, DWORD /*flags*/)
        {
            return src;
        }

        template<>
        std::string wm_cast<std::string, std::string>(const std::string& src, UINT /*codePage*/, DWORD /*flags*/)
        {
            return src;
        }

        template<>
        std::string wm_cast<std::string, wchar_t>(const wchar_t* src, UINT codePage, DWORD flags)
        {
            if (codePage == CP51932)
            {
                return unicodeToEUC(src, codePage);
            }
            //if (codePage == CP_EUC_BECKY){
            //動作せず。Becky!側のEUC_JP()で落ちる。2013/03/02
            //    return unicodeToEUC_BK(src, codePage);
            //}
            return wideToMulti(src, codePage, flags);
        }

        template<>
        std::wstring wm_cast<std::wstring, char>(const char* src, UINT codePage, DWORD flags)
        {
            if (codePage == CP51932)
            {
                return eucToUNICODE(src, codePage);
            }
            //if (codePage == CP_EUC_BECKY){
            //動作せず。Becky!側のEUC_JP()で落ちる。2013/03/02
            //    codePage = CP_ACP;
            //    return eucToUNICODE_BK(src, codePage);
            //}
            return multiToWide(src, codePage, flags);
        }

        template<>
        std::wstring wm_cast<std::wstring, wchar_t>(const wchar_t* src, UINT /*codePage*/, DWORD /*flags*/)
        {
            return src;
        }

        template<>
        std::string wm_cast<std::string, char>(const char* src, UINT /*codePage*/, DWORD /*flags*/)
        {
            return src;
        }

    }
}
