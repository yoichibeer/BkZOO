/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <BeckyAPI.h>
extern CBeckyAPI beckyApi;

#include "setting/SettingDlg.h"
#include "setting/DetectBeckyNewVersionDlg.h"
#include "menu/CustomMenu.h"
#include "bkzoo/util/BeckyVersion.h"
#include "bkzoo/util/MessageBox.h"
#include "bkzoo/config/ConfigRepository.h"
#include "bkzoo_config.h"
#include "bkzoo_string.h"
#include "bkzoo_log.h"

#include <utility>

#include "for_debug/detect_memory_leak.h"

using namespace bkzoo;



static std::string getXMailer()
{
    char x_mailer[_MAX_PATH] = { 0 };
    ::beckyApi.GetSpecifiedHeader("X-Mailer", x_mailer, _MAX_PATH);
    std::string xMailer(x_mailer);

    return std::move(xMailer);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks from Becky!

#ifdef __cplusplus
extern "C" {
#endif

    ////////////////////////////////////////////////////////////////////////
    // Called when menu is initialized.
    // cppcheck-suppress unusedFunction
    int WINAPI BKC_OnMenuInit(HWND hWnd, HMENU hMenu, int nType)
    {
        switch (nType)
        {
            //case BKC_MENU_MAIN:
            //	//メインウィンドウのメニューバー
            //	break;
            //case BKC_MENU_LISTVIEW:
            //	//リストビューのコンテキストメニュー
            //	break;
            //case BKC_MENU_TREEVIEW:
            //	//ツリービューのコンテキストメニュー
            //	break;
        case BKC_MENU_MSGVIEW:
            //メッセージビューのコンテキストメニュー
        {
            bkzoo::menu::createCustomMenu(hWnd, hMenu, false, nType);
        }
        break;
        case BKC_MENU_MSGEDIT:
            //編集可能時のメッセージビューウィンドウのコンテキストメニュー
        {
            bkzoo::menu::createCustomMenu(hWnd, hMenu, true, nType);
        }
        break;
        //case BKC_MENU_TASKTRAY:
        //	//タスクトレイアイコンのポップアップメニュー
        //	break;
        //case BKC_MENU_COMPOSE:
        //	//メール作成ウィンドウのメニューバー
        //	break;
        case BKC_MENU_COMPEDIT:
            //メール作成ウィンドウの編集ウィンドウのコンテキストメニュー
        {
            bkzoo::menu::createCustomMenu(hWnd, hMenu, true, nType);
        }
        break;
        case BKC_MENU_COMPREF:
            //メール作成ウィンドウの参照ウィンドウのコンテキストメニュー
        {
            bkzoo::menu::createCustomMenu(hWnd, hMenu, false, nType);
        }
        break;
        default:
            break;
        }
        // Always return 0.
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when plug-in setup is needed.
    // cppcheck-suppress unusedFunction
    int WINAPI BKC_OnPlugInSetup(HWND /* hWnd */)
    {
        bkzoo::setting::SettingDlg dlg;

        dlg.DoModal();// 返り値に関係なく1を返すため返り値はみない

        // Return nonzero if you have processed.
        return 1;
    }

    ////////////////////////////////////////////////////////////////////////
    // Called when plug-in information is being retrieved.
    typedef struct tagBKPLUGININFO
    {
        char szPlugInName[80];                 // Name of the plug-in
        char szVendor[80];                 // Name of the vendor
        char szVersion[80];                 // Version string
        char szDescription[256];                 // Short description about this plugin
    } BKPLUGININFO, *LPBKPLUGININFO;


    // cppcheck-suppress unusedFunction
    int WINAPI BKC_OnPlugInInfo(LPBKPLUGININFO lpPlugInInfo)
    {
        ::strcpy(lpPlugInInfo->szPlugInName, "BkZOO! URL OPEN and SEARCH");
        ::strcpy(lpPlugInInfo->szVendor, "yoichibeer");
        ::strcpy(lpPlugInInfo->szVersion, "Version 2.4.0");
        ::strcpy(lpPlugInInfo->szDescription,
            "Enables Becky to perform various operations to the selected text.");

        // Always return 0.
        return 0;
    }

    // cppcheck-suppress unusedFunction
    int WINAPI BKC_OnExit()
    {
        // メモリ上に保持していたBkZOO!の各種設定値をiniファイルに保存
        config::ConfigRepository::instance().saveConfig(Config::instance());

        if (!config::Config::instance().checkBeckyUpdate())
            return 0; // Becky!更新チェックが有効でなければ抜ける


        //
        // Becky! 更新チェック実行
        //

        // 自分の becky version
        util::BeckyVersion myselfBeckyVersion(util::BeckyVersion::getBeckyVersionOfMyself(beckyApi.GetVersion()));

        // これまで見た中で一番新しい becky version
        util::BeckyVersion latestBeckyVersion(util::BeckyVersion::getBeckyVersionLatest(beckyApi.GetVersion()));

        if (myselfBeckyVersion >= latestBeckyVersion)
            return 0;

        // 新しいバージョンが見つかったダイアログを表示する
        //HWND mainHandle = nullptr;
     //   HWND treeHandle = nullptr;
     //   HWND listHandle = nullptr;
     //   HWND viewHandle = nullptr;
     //   if (beckyApi.GetWindowHandles(&mainHandle, &treeHandle, &listHandle, &viewHandle) == FALSE)
     //   {
     //       LOG_ERROR << "failed GetWindowHandles()";
     //       return 0;
     //   }
        bkzoo::setting::DetectBeckyNewVersionDlg dlg;
        dlg.DoModal();// 返り値に関係なく1を返すため返り値はみない

        // 上記ダイアログで、このバージョンをスキップするのチェックを保存。
        config::ConfigRepository::instance().saveConfig(Config::instance());

        return 0;
    }

    static void updateConfigBeckyVersion()
    {
        // カレントメールのX-Mailerから becky version があれば取得
        const std::string currentMailVersion(util::BeckyVersion::extractBeckyVersionFrom(getXMailer()));
        if (!util::BeckyVersion::isValidBeckyVersion(currentMailVersion))
        {
            LOG_INFO << "isValidBeckyVersion(currentMailVersion) is false : currentMailVersion=" << currentMailVersion;
            return;
        }
        util::BeckyVersion currentMailBeckyVersion;
        currentMailBeckyVersion.initialize(currentMailVersion);

        // 自分の becky version
        util::BeckyVersion myselfBeckyVersion(util::BeckyVersion::getBeckyVersionOfMyself(beckyApi.GetVersion()));

        // これまで見た中で一番新しい becky version
        util::BeckyVersion latestBeckyVersion(util::BeckyVersion::getBeckyVersionLatest(beckyApi.GetVersion()));

        // カレントメールのベッキーバージョンが一番新しいときに、Config::versionを更新
        if ((currentMailBeckyVersion > myselfBeckyVersion)
            && (currentMailBeckyVersion > latestBeckyVersion))
        {
            config::Config::instance().setVersion(wm_cast<std::wstring>(currentMailBeckyVersion.version()));
        }
    }

    // cppcheck-suppress unusedFunction
    int WINAPI BKC_OnRetrieve(LPCSTR /*lpMessage*/, LPCSTR /*lpMailID*/)
    {
        updateConfigBeckyVersion();
        return 0;
    }

    // cppcheck-suppress unusedFunction
    int WINAPI BKC_OnOpenMail(LPCSTR /*lpMailID*/)
    {
        updateConfigBeckyVersion();
        return 0;
    }

#ifdef __cplusplus
}
#endif
