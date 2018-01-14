/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#include "CustomMenu.h"

#include "MenuInserter.h"
#include "MenuInserterImpl.h"

#include "bkzoo_util.h"
#include "bkzoo_config.h"

#include "bkzoo_log.h"

#include <cassert>
#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace menu
    {

        // ユーザ指定によりMenuの表示順を変えたい場合、設定画面で順番を指定するようにし、ここでMenuInserterインスタンスをリストにいれて順番いれかえる。

        BOOL createCustomMenu(HWND hWnd, HMENU appendedMenu, bool isEditableView, int nType)
        {
            try
            {
                //
                // Viewが編集可能の場合に追加するメニュー
                //

                // 挿入位置
                int insertPos = 0;
                int baseInsertPos = insertPos;

                bool success = 0;

                if (isEditableView)
                {
                    PastFileKeywordMenu pastFileKeywordMenu(hWnd, nType);
                    success = pastFileKeywordMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // Separatorを貼り付けと選択文字列操作の間に挿入
                Separator separator;
                if (insertPos > baseInsertPos) // baseより増えていたら追加されていたということ
                {
                    success = separator.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                //
                // 選択文字列が存在する場合に追加するメニュー
                //

                // 選択文字列があるか？（選択文字列からは空白等は除去）
                const std::wstring selectedString = StringGetter::removedSpaceText(StringGetter::selectedText(hWnd));
                if (selectedString.empty())
                {
                    return TRUE;
                }

                // 選択文字列を表示用に加工（文字列の長さが長い場合に短縮形にする）
                // isEditableViewのコンテキストメニューのサイズが固定なのをうけた回避策↓
                const std::wstring selectedStringShort = (isEditableView)
                    ? StringGetter::limitedLengthText(selectedString, ShortLength::SelectedString, ShortLength::LastStringLen)
                    : StringGetter::limitedLengthText(selectedString);


                // 挿入位置をリセット
                insertPos = 0;
                // baseInsertPos = insertPos; // 0のままで代入不要なのでコメントアウト


                // 任意サイト（Google, Yahoo!オークション, ebay, etc...）
                {
                    const auto& sites = Config::instance().sites();
                    MenuCommandIdSiteMap menuCommandIdSiteMap;
                    for (const auto& site : sites)
                    {
                        if (!site.enabled_bool())
                            continue;

                        SiteMenu siteMenu(nType, selectedString, selectedStringShort, isEditableView, site);
                        success = siteMenu.insert(appendedMenu, insertPos);
                        if (success)
                        {
                            ++insertPos;
                            menuCommandIdSiteMap.insert(std::pair<const UINT, const Site&>(siteMenu.getMenuCommandId(), site));
                        }
                    }
                    Config::instance().setMenuCommandIdSiteMap(menuCommandIdSiteMap);
                }


                // Separatorを検索とURLの間に挿入
                if (insertPos > baseInsertPos)
                {
                    success = separator.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                    baseInsertPos = insertPos; // reset
                }


                // ブラウザで開く
                {
                    BrowzerMenu browzerMenu(nType, selectedString, selectedStringShort);
                    success = browzerMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // IEで開く（デフォルトブラウザがIEなら表示しない）
                {
                    InternetExplorerMenu ieMenu(nType, selectedString, selectedStringShort);
                    success = ieMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // エクスプローラで開く
                {
                    ExplorerMenu explorerMenu(nType, selectedString, selectedStringShort);
                    success = explorerMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // TortoiseSVNのリポジトリブラウザで開く
                {
                    TortoiseSVNMenu tortoiseSVNMenu(nType, selectedString, selectedStringShort);
                    success = tortoiseSVNMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // FFFTPで開く
                {
                    FFFTPMenu ffftpMenu(nType, selectedString, selectedStringShort);
                    success = ffftpMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // FileZillaで開く
                {
                    FileZillaMenu fileZillaMenu(nType, selectedString, selectedStringShort);
                    success = fileZillaMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // WinSCPで開く
                {
                    WinSCPMenu winSCPMenu(nType, selectedString, selectedStringShort);
                    success = winSCPMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // TeraTermで開く
                {
                    TeraTermMenu teraTermMenu(nType, selectedString, selectedStringShort);
                    success = teraTermMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // リモートデスクトップで開く
                {
                    RemoteDesktopMenu remoteDesktopMenu(nType, selectedString, selectedStringShort);
                    success = remoteDesktopMenu.insert(appendedMenu, insertPos);
                    if (success) ++insertPos;
                }


                // Separator 最後に挿入
                if (insertPos > baseInsertPos)
                {
                    // success = 
                    separator.insert(appendedMenu, insertPos/*++*/);
                    // if (success) ++insertPos;
                }

                return TRUE;
            }
            catch (...)
            {
                LOG_ERROR << "createCustomMenu hWnd=" << hWnd << ", appendedMenu=" << appendedMenu << ", isEditableView=" << isEditableView << ", nType=" << nType;
                assert(false);
                return FALSE;
            }
        }

    }
}
