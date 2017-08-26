/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_MENU_MENU_INSERTER_IMPL_H
#define BKZ_MENU_MENU_INSERTER_IMPL_H

#include "MenuInserter.h"

#include "bkzoo_config.h"

namespace bkzoo
{
    namespace menu
    {

        // メニューに表示する文字列の長さを制限する.
        // editableViewのコンテキストメニューのサイズが固定なのをうけた回避策
        enum ShortLength : size_t
        {
            SelectedString = 10, // 試行錯誤した結果の数値
            LastStringLen = 3, // 試行錯誤した結果の数値
            Title = 31, // 試行錯誤した結果の数値。この値よりも大きくしてもBecky!本体で切り捨てるため表示されない。
        };


        class PastFileKeywordMenu : public MenuInserter
        {
        public:
            PastFileKeywordMenu(HWND hWnd, int nType);
        };


        class GoogleMenu : public MenuInserter
        {
        public:
            GoogleMenu(int nType, const std::wstring& selectedShortString);
        };


        class SiteMenu : public MenuInserter
        {
        public:
            SiteMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString, bool editableView, const Site& site);
            UINT getMenuCommandId() const;

        private:
            UINT menuCommandId_;
        };


        class BrowzerMenu : public MenuInserter
        {
        public:
            BrowzerMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };


        class ExplorerMenu : public MenuInserter
        {
        public:
            ExplorerMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };


        class TortoiseSVNMenu : public MenuInserter
        {
        public:
            TortoiseSVNMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };


        class FFFTPMenu : public MenuInserter
        {
        public:
            FFFTPMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };


        class FileZillaMenu : public MenuInserter
        {
        public:
            FileZillaMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };


        class WinSCPMenu : public MenuInserter
        {
        public:
            WinSCPMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };


        class TeraTermMenu : public MenuInserter
        {
        public:
            TeraTermMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };


        class RemoteDesktopMenu : public MenuInserter
        {
        public:
            RemoteDesktopMenu(int nType, const std::wstring& selectedString, const std::wstring& selectedShortString);
        };

    }
}

#endif // BKZ_MENU_MENU_INSERTER_IMPL_H
