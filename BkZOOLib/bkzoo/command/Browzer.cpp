/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Browzer.h"

#include "bkzoo_url.h"
#include "util/MessageBox.h"

#include "resource.h"

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        BrowzerCommand::BrowzerCommand(const SimpleParam& param)
            : param_(param), m_availableSchemes(Scheme::HTTP | Scheme::HTTPS | Scheme::FTP)
        {
        }
        BrowzerCommand::BrowzerCommand(const SimpleParam& param, unsigned long availableSchemes)
            : param_(param), m_availableSchemes(availableSchemes)
        {
        }

        bool BrowzerCommand::execute()
        {
            const url::URL url(param_.string, m_availableSchemes);

            if (!url.validate())
            {
                return false;
            }

            // 警告表示
            if (!util::okCancelWarningMessageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_WARNING_OPEN_URL))
            {
                // skip
                return true;
            }

            // todo: 以下、ftp の場合にFTPクライアントが立ち上がる場合あるが、Becky!の　ftp://xxx.xxx のリンクと同じ動作。とりあえずそのまま。
            ::ShellExecute(param_.hWnd, L"open", url.getURL().c_str(), nullptr, nullptr, SW_SHOW);

            return true;
        }

    }
}
