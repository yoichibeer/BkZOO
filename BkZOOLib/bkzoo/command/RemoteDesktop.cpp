/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "RemoteDesktop.h"
#include "util/MessageBox.h"

#include "bkzoo_url.h"
#include "bkzoo_util.h"

#include "resource.h"

#include <sstream>

#include <cassert>
#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        RemoteDesktopCommand::RemoteDesktopCommand(const SimpleParam& param)
            : param_(param)
        {
        }

        bool RemoteDesktopCommand::execute()
        {
            url::URL url(param_.string, Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
            if (!url.validate())
            {
                return false;
            }

            // 警告表示
            //if (!util::okCancelWarningMessageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_WARNING_OPEN_URL))
            //{
            //    // skip
            //    return true;
            //}

            // mstsc.exe /v ip or domain
            std::wostringstream option;
            option << L" /v:" << url.getURL();
            ::ShellExecute(param_.hWnd, nullptr, L"mstsc", option.str().c_str(), nullptr, SW_SHOW);

            return true;
        }

    }
}
