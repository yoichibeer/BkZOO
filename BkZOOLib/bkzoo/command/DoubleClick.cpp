/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "DoubleClick.h"

#include "Google.h"
#include "PasteFileKeyword.h"
#include "SearchWord.h"
#include "util/ModifierKeyType.h"
#include "helper/ClickHelper.h"

#include "util/TripleClickDetector.h"

#include "bkzoo_util.h"
#include "bkzoo_config.h"
#include "bkzoo_log.h"

#include <regex>

#include "for_debug/detect_memory_leak.h"

extern HMODULE hModuleBkZOO;
extern ::bkzoo::util::TripleClickDetector tripleClickDetector;

namespace bkzoo
{
    namespace command
    {

        DoubleClickCommand::DoubleClickCommand(const DoubleClickParam& param)
            : param_(param)
        {
        }

        bool DoubleClickCommand::execute()
        {
            if (::tripleClickDetector.running())
            {
                // TripleClick確定
                // 処理はWM_LBUTTONDOWNに任せて、ここは何もしないで抜ける
                LOG_INFO << "DoubleClickCommand::execute() Not DoubleClick! It is TripleClick!";
                return false;
            }

            //
            // 以下、DoubleClick確定
            //

            if ((param_.modifierKey & ModifierKeyType::Ctrl) == 0)
            {
                // Ctrl以外は抜ける
                return false;
            }

            if (!Config::instance().ctrlDoubleClick())
            {
                // 設定ファイルでCtrl+DoubleClickが有効でないなら抜ける
                return false;
            }

            // DoubleClickの時はこのタイミングでしか選択文字列を取得できない
            const std::wstring selectedText = StringGetter::selectedText(param_.hWnd);
            const std::wstring spaceRemovedSelectedStr = StringGetter::removedSpaceText(selectedText);
            if (spaceRemovedSelectedStr.empty())
            {
                // 選択文字列が空のときは貼り付け実施
                const std::wstring clipboardText = StringGetter::clipboardText(param_.hWnd);
                const std::wstring clipboardStr = StringGetter::removedSpaceText(clipboardText);
                if (clipboardStr.empty())
                {
                    // クリップボードも空ならば抜ける
                    if (selectedText.empty() && clipboardText.empty())
                        LOG_ERROR << "DoubleClickCommand::execute() selectedText.empty() && clipboardText.empty()";
                    return false;
                }

                PasteFileKeywordCommand pasteFileKeyword(SimpleParam{ param_.hWnd, clipboardStr });
                return pasteFileKeyword.execute();
            }

            // regexpString指定サイト検索
            if (helper::ClickHelper::executeRegexpSite(param_.hWnd, spaceRemovedSelectedStr))
            {
                return true;
            }

            // 最後にgoogle。
            GoogleCommand google(SimpleParam{ param_.hWnd, spaceRemovedSelectedStr });
            return google.execute();
        }
    }
}
