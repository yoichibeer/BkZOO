/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#include "CallBack.h"

#include "command/Apps.h"
#include "command/Google.h"
#include "command/PasteFileKeyword.h"
#include "command/SearchWord.h"
#include "command/TortoiseSVN.h"
#include "command/RemoteDesktop.h"
#include "command/Browzer.h"
#include "command/InternetExplore.h"
#include "command/Explorer.h"

#include "thread_func/ThreadFunc.h"

#include "util/MessageBox.h"
#include "bkzoo_util.h"

#include "resource.h"

#include "bkzoo_log.h"
#include "bkzoo_string.h"
#include "bkzoo_config.h"

#include <process.h>

#include <cassert>
#include "debug/detect_memory_leak.h"


namespace bkzoo
{
    namespace callback
    {

        void WINAPI pasteFileKeyword(HWND hWnd, LPARAM /*lParam*/)
        {
            try
            {
                command::Command* command = new command::PasteFileKeywordCommand(
                    command::SimpleParam{ hWnd, StringGetter::removedSpaceText(StringGetter::clipboardText(hWnd)) });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }


        void WINAPI googleSearch(HWND hWnd, LPARAM /*lParam*/)
        {
            try
            {
                command::Command* command = new command::GoogleCommand(
                    command::SimpleParam{ hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd)) });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }


        static void executeApp(command::AppsParam param)
        {
            try
            {
                command::Command* command = new command::AppsCommand(param);
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << ", selectedString=" << param.selectedString << ", fileName=" << param.installLocationFileName;
                assert(false);
            }
        }

        void WINAPI ffftp(HWND hWnd, LPARAM /*lParam*/)
        {
            executeApp(command::AppsParam::createParamOfFFFTP(hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd))));
        }

        void WINAPI fileZilla(HWND hWnd, LPARAM /*lParam*/)
        {
            executeApp(command::AppsParam::createParamOfFileZilla(hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd))));
        }

        void WINAPI winSCP(HWND hWnd, LPARAM /*lParam*/)
        {
            executeApp(command::AppsParam::createParamOfWinSCP(hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd))));
        }

        void WINAPI teraTerm(HWND hWnd, LPARAM /*lParam*/)
        {
            executeApp(command::AppsParam::createParamOfTeraTerm(hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd))));
        }

        void WINAPI svn(HWND hWnd, LPARAM /*lParam*/)
        {
            try
            {
                command::Command* command = new command::TortoiseSVNCommand(
                    command::SimpleParam{ hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd)) });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }

        void WINAPI remoteDesktop(HWND hWnd, LPARAM /*lParam*/)
        {
            try
            {
                command::Command* command = new command::RemoteDesktopCommand(
                    command::SimpleParam{ hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd)) });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }

        void WINAPI browzer(HWND hWnd, LPARAM /*lParam*/)
        {
            try
            {
                command::Command* command = new command::BrowzerCommand(
                    command::SimpleParam{ hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd)) });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }

        void WINAPI internetExplore(HWND hWnd, LPARAM /*lParam*/)
        {
            try
            {
                command::Command* command = new command::InternetExploreCommand(
                    command::SimpleParam{ hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd)) });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }

        void WINAPI explorer(HWND hWnd, LPARAM /*lParam*/)
        {
            try
            {
                command::Command* command = new command::ExplorerCommand(
                    command::SimpleParam{ hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd)) });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }

        void WINAPI site(HWND hWnd, LPARAM lParam)
        {
            try
            {
                // 選択したメニューIDから対応するSiteを検索
                const WORD menuCommandID = LOWORD(lParam);
                const MenuCommandIdSiteMap& siteMapping = Config::instance().menuCommandIdSiteMap();
                const MenuCommandIdSiteMap::const_iterator it = siteMapping.find(menuCommandID);
                if (it == siteMapping.end())
                {
                    // 見つからない場合は終了
                    return;
                }

                const Site& site = it->second;

                // urlがなければ終了
                if (site.url().empty())
                {
                    messageBox(hWnd, IDS_STRING_ERR_MESSAGE_NO_SITE_URL);
                    return;
                }

                command::Command* command = new command::SearchWordCommand(
                    command::SearchWordCommand::Param{ hWnd, StringGetter::removedSpaceText(StringGetter::selectedText(hWnd)), site.regex(), site.url(), site.encode_int(), site.method_enum(), url::urlencode });
                ::_beginthread(thread_func::executeCommand, 0, command);
            }
            catch (...)
            {
                LOG_ERROR << "catch";
                assert(false);
            }
        }

    }
}
