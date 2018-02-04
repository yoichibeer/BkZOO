/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "MenuInserter.h"

#include <BeckyAPI.h>

#include <sstream>

#include "for_debug/detect_memory_leak.h"

extern CBeckyAPI beckyApi;

namespace bkzoo
{
    namespace menu
    {

        static MENUITEMINFO* createMenuItemInfo(const std::wstring& title, UINT menuCommandID)
        {
            MENUITEMINFO* pMenuItemInfo = new MENUITEMINFO({ 0 });
            pMenuItemInfo->cbSize = sizeof(MENUITEMINFO);
            pMenuItemInfo->fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID;
            pMenuItemInfo->fType = MFT_STRING;
            pMenuItemInfo->dwTypeData = const_cast<LPWSTR>(title.c_str());
            pMenuItemInfo->wID = menuCommandID;
            return pMenuItemInfo;
        }

        bool MenuInserter::insert(HMENU hMenu, UINT item)
        {
            if (!validate()) return false;

            BOOL ret = ::InsertMenuItem(hMenu, item, TRUE, menuItemInfo_.get());
            if (ret == FALSE) return false;

            return true;
        }

        bool MenuInserter::validate() const
        {
            if (menuItemInfo_.get() == nullptr) return false;
            return true;
        }

        UINT MenuInserter::initializeMenu(
            int nType,
            const std::wstring& lhsString,
            const std::wstring& title,
            const std::wstring& rhsString,
            wchar_t shortcutKey,
            const std::string& statusBarString,
            void (CALLBACK* lpCallback)(HWND, LPARAM))
        {
            std::wostringstream wos;
            wos << lhsString << title << rhsString;
            if ('\0' != shortcutKey)
            {
                wos << L"(&" << shortcutKey << L")";
            }
            title_ = wos.str();

            std::string statusbar("BkZOO! : ");
            statusbar += statusBarString;

            const UINT menuCommandID = ::beckyApi.RegisterCommand(statusbar.c_str(), nType, lpCallback);

            menuItemInfo_.reset(createMenuItemInfo(title_, menuCommandID));

            return menuCommandID;
        }



        static MENUITEMINFO* createSeparatorMenuItemInfo()
        {
            MENUITEMINFO* pMenuItemInfo = new MENUITEMINFO({ 0 });
            pMenuItemInfo->cbSize = sizeof(MENUITEMINFO);
            pMenuItemInfo->fMask = MIIM_FTYPE;
            pMenuItemInfo->fType = MFT_SEPARATOR;
            return pMenuItemInfo;
        }


        Separator::Separator()
        {
            menuItemInfo_.reset(createSeparatorMenuItemInfo());
        }

    }
}