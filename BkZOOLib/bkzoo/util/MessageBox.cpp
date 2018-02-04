/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "util/MessageBox.h"

#include "util/GetResourceString.h"
#include "registry\Registry.h"
#include "bkzoo_config.h"

#include <Shlwapi.h>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace util
    {
        bool okCancelWarningMessageBox(HWND hWnd, UINT uID)
        {
            std::wstring message = getResourceStringW(uID);
            const int result = ::SHMessageBoxCheck(hWnd, message.c_str(), L"BkZOO!", MB_OKCANCEL | MB_ICONWARNING, IDOK, registry::Registry::REG_VALUE_GUID);
            return (result == IDOK);
        }

        void messageBox(HWND hWnd, UINT uID)
        {
            std::wstring message = getResourceStringW(uID);
            ::MessageBox(hWnd, message.c_str(), L"BkZOO!", MB_OK | MB_ICONINFORMATION);
        }

    }
}
