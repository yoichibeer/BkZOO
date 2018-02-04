/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "TortoiseSVN.h"

#include "registry/Registry.h"
#include "util/MessageBox.h"

#include "bkzoo_url.h"
#include "bkzoo_util.h"

#include "resource.h"

#include <sstream>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        TortoiseSVNCommand::TortoiseSVNCommand(const SimpleParam& param)
            : param_(param)
        {
        }

        bool TortoiseSVNCommand::execute()
        {
            std::wstring installLocation;
            bool exists = registry::Registry::getTortoiseProcInstallLocation(&installLocation);
            if (!exists)
            {
                return false;
            }

            url::URL url(param_.string, Scheme::HTTP | Scheme::HTTPS | Scheme::SVN);
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

            // TortoiseProc.exe /command:repobrowser /path:"url/to/repository"
            std::wostringstream option;
            option << L"/command:repobrowser /path:\"" << url.getURL() << L"\"";
            ::ShellExecute(param_.hWnd, nullptr, installLocation.c_str(),
                option.str().c_str(), nullptr, SW_SHOW);

            return true;
        }

    }
}
