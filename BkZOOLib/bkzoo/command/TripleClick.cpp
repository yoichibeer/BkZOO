/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "TripleClick.h"

#include "Browzer.h"
#include "Explorer.h"
#include "TortoiseSVN.h"
#include "Apps.h"
#include "PasteFileKeyword.h"
#include "helper/ClickHelper.h"
#include "util/ModifierKeyType.h"

#include "bkzoo_url.h"
#include "bkzoo_config.h"
#include "bkzoo_string.h"
#include "bkzoo_util.h"
#include "bkzoo_log.h"

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        TripleClickCommand::TripleClickCommand(const TripleClickParam& param)
            : param_(param)
        {
        }

        // アプリケーションに優先順位を付けたい場合は設定画面で優先順位を指定するようにして、ここでCommandインスタンスをリストにいれて順番いれかえる。

        bool TripleClickCommand::execute()
        {
            SimpleParam& simpleParam = param_.simpleParam;

            // ここで選択文字列を取得する。取得できなかったらここで抜ける。
            int failedCount = 0;
            while (true)
            {
                // Danaのトリプルクリック後の文字列選択が完了していない場合があるようなので50ms待って取得しなおす。
                Sleep(50);

                // TripleClickの時はこのタイミングでしか選択文字列を取得できない
                const std::wstring selectedText = StringGetter::selectedText(simpleParam.hWnd);
                simpleParam.string = StringGetter::removedSpaceText(selectedText);
                if (!simpleParam.string.empty())
                {

                    LOG_DEBUG << "TripleClickCommand::execute():param_.string=" << wm_cast<std::string>(simpleParam.string) << ", param_.hWnd=" << simpleParam.hWnd << ", failedCount=" << failedCount;
                    break;
                }

                // 3回繰り返す。
                if (++failedCount == 3)
                {
                    if (StringGetter::selectedText(simpleParam.hWnd).empty())
                        LOG_WARNING << __FUNCTION__ << ": param_.string.empty(), param_.hWnd=" << simpleParam.hWnd << ", failedCount=" << failedCount;
                    return false;
                }
            }



            {
                // Ctrlキー押しながらトリプルクリックしたときの処理
                if (Config::instance().ctrlTripleClick()
                    && ((param_.modifierKey & ModifierKeyType::Ctrl) != 0))
                {
                    // Ctrlキーがあれば <file:// > を付加。
                    PasteFileKeywordCommand command(simpleParam);
                    if (command.execute())
                    {
                        return true;
                    }
                    else
                    {
                        // 処理できなければここでreturnしないで以下の処理を続ける。
                        // つまりCtrlキーを押していてもトリプルクリックと同じ処理をするということ。
                    }
                }
            }


            if (!Config::instance().tripleClick())
            {
                // 設定ファイルでTripleClickが有効でないなら抜ける
                return false;
            }


            // 以下はCtrlが押されていてもいなくても処理をする。
            {
                // ブラウザで開く (http, https)
                BrowzerCommand command(simpleParam, Scheme::HTTP | Scheme::HTTPS);
                if (command.execute())
                {
                    return true;
                }
            }
            {
                // Explorerで開く
                ExplorerCommand command(simpleParam);
                if (command.execute())
                {
                    return true;
                }
            }
            {
                // TortoiseSVNで開く
                TortoiseSVNCommand command(simpleParam);
                if (command.execute())
                {
                    return true;
                }
            }
            {
                // FFFTPで開く
                AppsCommand command(AppsParam::createParamOfFFFTP(simpleParam.hWnd, simpleParam.string));
                if (command.execute())
                {
                    return true;
                }
            }
            {
                // FileZillaで開く
                AppsCommand command(AppsParam::createParamOfFileZilla(simpleParam.hWnd, simpleParam.string));
                if (command.execute())
                {
                    return true;
                }
            }
            {
                // WinSCPで開く
                AppsCommand command(AppsParam::createParamOfWinSCP(simpleParam.hWnd, simpleParam.string));
                if (command.execute())
                {
                    return true;
                }
            }
            {
                // TeraTermで開く
                AppsCommand command(AppsParam::createParamOfTeraTerm(simpleParam.hWnd, simpleParam.string));
                if (command.execute())
                {
                    return true;
                }
            }
            {
                // ブラウザで開く (ftp)
                BrowzerCommand command(simpleParam, Scheme::FTP);
                if (command.execute())
                {
                    return true;
                }
            }

            // regexpString指定サイト検索
            if (helper::ClickHelper::executeRegexpSite(simpleParam.hWnd, simpleParam.string))
            {
                return true;
            }

            return false;
        }

    }
}
