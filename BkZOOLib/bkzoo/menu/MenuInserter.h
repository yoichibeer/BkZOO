/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_MENU_MENU_INSERTER_H
#define BKZ_MENU_MENU_INSERTER_H

#include <Windows.h>

#include <string>
#include <memory>

#include "defs.h"

namespace bkzoo
{
    namespace menu
    {

        /// @todo (yoichi) 継承やめることを検討する。staticにできるメソッドばかり？
        class MenuInserter
        {
        public:
            MenuInserter() = default;
            virtual ~MenuInserter() = default;
            bool insert(HMENU hMenu, UINT item); // 継承しない

        protected:
            std::unique_ptr<MENUITEMINFO> menuItemInfo_;
            std::wstring title_;

        protected:
            UINT initializeMenu(
                int nType,
                const std::wstring& lhsString,
                const std::wstring& title,
                const std::wstring& rhsString,
                wchar_t shortcutKey,
                const std::string& statusBarString,
                void (CALLBACK* lpCallback)(HWND, LPARAM)
            ); // 継承しない

        private:
            bool validate() const; // 継承しない
            DISALLOW_COPY_AND_ASSIGN(MenuInserter);
        };


        class Separator : public MenuInserter
        {
        public:
            Separator();
        };

    }
}

#endif // BKZ_MENU_MENU_INSERTER_H
