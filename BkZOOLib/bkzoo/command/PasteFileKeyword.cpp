/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "PasteFileKeyword.h"

#include "clipboard/Clipboard.h"
#include "bkzoo_url.h"
#include "bkzoo_util.h"

#include <sstream>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        PasteFileKeywordCommand::PasteFileKeywordCommand(const SimpleParam& param)
            : param_(param)
        {
        }

        bool PasteFileKeywordCommand::execute()
        {
            url::URL url(
                param_.string,
                Scheme::HTTP | Scheme::HTTPS | Scheme::FTP |
                Scheme::FILE_URI_LOCALPATH | Scheme::FILE_URI_UNC);
            if (!url.validate())
            {
                return false;
            }

            // < >を付加
            const std::wstring url_str = url.getURL();
            std::wostringstream ostr;
            ostr << L"<" << url_str << ">\n";

            // クリップボードにセットして貼り付け（クリップボードは現状復帰する）
            clipboard::RevertClipboard revertClipboard(param_.hWnd);
            clipboard::ScopedClipboard scopedClipboard(param_.hWnd);
            scopedClipboard.setClipboardData(ostr.str());
            scopedClipboard.close();
            ::SendMessage(param_.hWnd, WM_COMMAND, 0xE125, 0); // 貼り付けコマンド送信
            revertClipboard.revert();

            return true;
        }

    }
}
