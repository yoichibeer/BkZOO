/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Google.h"

#include "url/URLEncode.h"
#include "util/MessageBox.h"
#include "bkzoo_string.h"

#include "resource.h"

#include <sstream>
#include <string>

#include "for_debug/detect_memory_leak.h"

extern HMODULE hModuleBkZOO;

namespace bkzoo
{
    namespace command
    {

        GoogleCommand::GoogleCommand(const SimpleParam& param)
            : param_(param)
        {
        }

        bool GoogleCommand::execute()
        {
            if (!util::okCancelWarningMessageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_WARNING_OPEN_URL))
            {
                // skip
                return true;
            }

            std::string utf8String = wm_cast<std::string>(param_.string, CP_UTF8);
            std::string urlEncodedString = url::urlencode(utf8String);
            std::ostringstream openStr;
            char googleSearchParamsStr[_MAX_PATH] = { 0 };
            LoadStringA(::hModuleBkZOO, IDS_STRING_GOOGLE_SEARCH_PARAMS, googleSearchParamsStr, _MAX_PATH);
            openStr << googleSearchParamsStr << urlEncodedString.c_str();
            ::ShellExecuteA(param_.hWnd, "open", openStr.str().c_str(), NULL, NULL, SW_SHOW);

            return true;
        }

    }
}
