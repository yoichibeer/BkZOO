/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#include "MenuInserterImpl.h"

#include "callback/CallBack.h"
#include "registry/Registry.h"
#include "util/RegexSearcher.h"
#include "bkzoo_url.h"
#include "bkzoo_util.h"
#include "bkzoo_config.h"
#include "bkzoo_string.h"
#include "bkzoo_log.h"

#include "resource.h"

#include <filesystem>

#include "for_debug/detect_memory_leak.h"

extern HMODULE hModuleBkZOO;

namespace bkzoo
{
    namespace menu
    {

        static std::string getResoureStr(UINT uID)
        {
            char resourceStr[_MAX_PATH] = { 0 };
            ::LoadStringA(::hModuleBkZOO, uID, resourceStr, _MAX_PATH);
            return std::string(resourceStr);
        }

        PastFileKeywordMenu::PastFileKeywordMenu(HWND hWnd, int nType)
        {
            // <と>を付けて貼りつけ
            const std::wstring clipBoardString = StringGetter::removedSpaceText(StringGetter::clipboardText(hWnd));
            if (clipBoardString.empty()) return;

            const url::URL clipboardUrl(
                clipBoardString,
                Scheme::HTTP | Scheme::HTTPS | Scheme::FTP |
                Scheme::FILE_URI_LOCALPATH | Scheme::FILE_URI_UNC
            );
            if (!clipboardUrl.validate()) return;

            const std::wstring clipBoardShortString =
                StringGetter::limitedLengthText(clipboardUrl.getURL(), ShortLength::SelectedString, ShortLength::LastStringLen);

            initializeMenu(
                nType,
                L"<",
                clipBoardShortString,
                L"> を貼り付け",
                Config::instance().shortcut(App::PasteBracket),
                getResoureStr(IDS_STRING_MENU_STATUS_PASTE_FILE_KEYWORD),
                callback::pasteFileKeyword
            );
        }


        GoogleMenu::GoogleMenu(int nType, const std::wstring& selectedShortString)
        {
            initializeMenu(
                nType,
                L"Googleで検索:\"",
                selectedShortString,
                L"\"",
                Config::instance().shortcut(App::Google),
                getResoureStr(IDS_STRING_MENU_STATUS_GOOGLE_SEARCH),
                callback::googleSearch
            );
        }


        SiteMenu::SiteMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString, bool editableView, const Site& site)
            : menuCommandId_(0xFFFFFFFF)
        {
            util::RegexSearcher regexSearcher(selectedString, site.regex());
            if (!regexSearcher.validate()) return;

            //
            // メニュー文字列
            //
            size_t shortLength = 0; // 短縮する場合のサイズと短縮するかのflagも兼ねる
            if (editableView)  // editableViewの場合か、設定ファイルに記載のメニュー文字列が長すぎる場合に短縮する
            {
                if (ShortLength::Title < site.title().length())
                {
                    shortLength = ShortLength::Title;
                }
            }
            else
            {
                if ((_MAX_PATH / 2) < site.title().length())
                {
                    shortLength = _MAX_PATH / 2;
                }
            }

            std::wstring title(site.title());
            if (shortLength != 0)
            {
                // 短縮が必要と判断された場合に実施
                title.resize(shortLength);
            }

            menuCommandId_ = initializeMenu(
                nType,
                title.append(L":\""),
                selectedShortString,
                L"\"",
                site.shortcut_char(),
                wm_cast<std::string>(site.statusbar()),
                callback::site
            );
        }

        UINT SiteMenu::getMenuCommandId() const
        {
            return menuCommandId_;
        }


        BrowzerMenu::BrowzerMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const url::URL url(selectedString, Scheme::HTTP | Scheme::HTTPS | Scheme::FTP);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"ブラウザで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::Browzer),
                getResoureStr(IDS_STRING_MENU_STATUS_BROWZER),
                callback::browzer
            );
        }


        InternetExplorerMenu::InternetExplorerMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const url::URL url(selectedString, Scheme::HTTP | Scheme::HTTPS | Scheme::FTP);
            if (!url.validate()) return;

            // デフォルトブラウザがIEならIEのメニューは表示しない
            if (registry::Registry::isDefaultBrowzerIExplorer()) return;

            std::wstring iexploreInstallLocation;
            if (registry::Registry::getIexploreInstallLocation(&iexploreInstallLocation))
            {
                if (iexploreInstallLocation.empty())
                {
                    LOG_ERROR << "IEXPLORE.EXE is empty.";
                    return;
                }
            }
            else
            {
                LOG_ERROR << "getIexploreInstallLocation() failed.";
                return;
            }

            initializeMenu(
                nType,
                L"IEで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::InternetExplore),
                getResoureStr(IDS_STRING_MENU_STATUS_INTERNET_EXPLORER),
                callback::internetExplore
            );
        }


        ExplorerMenu::ExplorerMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const url::URL url(selectedString, Scheme::FILE_URI_LOCALPATH | Scheme::FILE_URI_UNC);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::Explorer),
                getResoureStr(IDS_STRING_MENU_STATUS_EXPLORER),
                callback::explorer
            );
        }


        TortoiseSVNMenu::TortoiseSVNMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const bool exists = registry::Registry::getTortoiseProcInstallLocation(nullptr);
            if (!exists) return;

            const url::URL url(selectedString, Scheme::HTTP | Scheme::HTTPS | Scheme::SVN);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"TortoiseSVNで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::TortoiseSVN),
                getResoureStr(IDS_STRING_MENU_STATUS_SVN),
                callback::svn
            );
        }


        FFFTPMenu::FFFTPMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const bool exists = registry::Registry::getInstallLocation(L"FFFTP", nullptr);
            if (!exists) return;

            const url::URL url(selectedString, Scheme::FTP);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"FFFTPで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::FFFTP),
                getResoureStr(IDS_STRING_MENU_STATUS_FFFTP),
                callback::ffftp
            );
        }


        FileZillaMenu::FileZillaMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const bool exists = registry::Registry::getInstallLocation(L"FileZilla Client", nullptr);
            if (!exists) return;

            const url::URL url(selectedString, Scheme::FTP | Scheme::FTPS | Scheme::SFTP);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"FileZillaで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::FileZilla),
                getResoureStr(IDS_STRING_MENU_STATUS_FILEZILLA),
                callback::fileZilla
            );
        }


        WinSCPMenu::WinSCPMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const bool exists = registry::Registry::getInstallLocation(L"winscp3_is1", nullptr);
            if (!exists) return;

            const url::URL url(selectedString, Scheme::FTP | Scheme::FTPS | Scheme::SCP);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"WinSCPで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::WinSCP),
                getResoureStr(IDS_STRING_MENU_STATUS_WINSCP),
                callback::winSCP
            );
        }


        TeraTermMenu::TeraTermMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const bool exists = registry::Registry::getInstallLocation(L"Tera Term_is1", nullptr);
            if (!exists) return;

            const url::URL url(selectedString, Scheme::TELNET);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"TeraTermで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::TeraTerm),
                getResoureStr(IDS_STRING_MENU_STATUS_TERATERM),
                callback::teraTerm
            );
        }


        static bool validateRemoteDesktop()
        {
            wchar_t path[_MAX_PATH] = { L'\0' };
            UINT ret = ::GetSystemDirectory(path, _MAX_PATH);
            if (ret == 0) return false;
            if (ret >= _MAX_PATH) return false;

            std::wstring mstsc(path);
            mstsc.append(L"\\mstsc.exe");
            const std::experimental::filesystem::path mstsc_path(mstsc);
            if (std::experimental::filesystem::exists(mstsc_path)) return true;

            return false;
        }

        RemoteDesktopMenu::RemoteDesktopMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString)
        {
            const bool exists = validateRemoteDesktop();
            if (!exists) return;

            const url::URL url(selectedString, Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
            if (!url.validate()) return;

            initializeMenu(
                nType,
                L"リモートデスクトップで\"",
                selectedShortString,
                L"\"を開く",
                Config::instance().shortcut(App::RemoteDesktop),
                getResoureStr(IDS_STRING_MENU_STATUS_REMOTE_DESKTOP),
                callback::remoteDesktop
            );
        }

    }
}