/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_SETTING_SEARCH_SITE_H
#define BKZ_SETTING_SEARCH_SITE_H

#include "Column.h"
#include "BackgroundColor.h"

#include "resource.h"
#include "wtlheader.h"

#include <vector>
#include <list>
#include <memory>

#include "defs.h"

namespace bkzoo
{
    namespace setting
    {

        class SearchSiteForm final : public CDialogImpl<SearchSiteForm>
        {
        public:
            enum { IDD = IDD_SEARCH_SITE_FORMVIEW };

            BEGIN_MSG_MAP(SearchSiteForm)
                MSG_WM_INITDIALOG(OnInitDialog)
                NOTIFY_HANDLER(IDC_LIST_VIEW, LVN_ITEMCHANGED, OnLvnItemchangedListView)
                NOTIFY_HANDLER(IDC_LIST_VIEW, NM_CUSTOMDRAW, OnNMCustomdrawListView)
                COMMAND_HANDLER(IDC_BUTTON_UP, BN_CLICKED, OnBnClickedButtonUp)
                COMMAND_HANDLER(IDC_BUTTON_DOWN, BN_CLICKED, OnBnClickedButtonDown)
                COMMAND_HANDLER(IDC_BUTTON_NEW, BN_CLICKED, OnBnClickedButtonNew)
                COMMAND_HANDLER(IDC_BUTTON_COPY, BN_CLICKED, OnBnClickedButtonCopy)
                COMMAND_HANDLER(IDC_BUTTON_DELETE, BN_CLICKED, OnBnClickedButtonDelete)
                COMMAND_HANDLER(IDC_COMBO_ENCODE, CBN_SELCHANGE, OnCbnSelchangeComboEncode)
                COMMAND_HANDLER(IDC_COMBO_METHOD, CBN_SELCHANGE, OnCbnSelchangeComboMethod)
                COMMAND_HANDLER(IDC_CHECK_CLICKABLE, BN_CLICKED, OnBnClickedCheckClickable)
                MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
                MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
                COMMAND_HANDLER(IDC_EDIT_REGEX, EN_CHANGE, OnEnChangeEditRegex)
                COMMAND_HANDLER(IDC_EDIT_TITLE, EN_CHANGE, OnEnChangeEditTitle)
                COMMAND_HANDLER(IDC_EDIT_SHORTCUT, EN_CHANGE, OnEnChangeEditShortcut)
                COMMAND_HANDLER(IDC_EDIT_URL, EN_CHANGE, OnEnChangeEditUrl)
                NOTIFY_HANDLER(IDC_LIST_VIEW, NM_DBLCLK, OnNMDblclkListView)
            END_MSG_MAP()

        public:
            SearchSiteForm() = default;
            ~SearchSiteForm() = default;

            // FormView初期化
            BOOL OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */);

            // FormView背景を白に変更
            LRESULT OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

            // FormView上のstaticコントロールの背景を白に変更
            LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

            LRESULT OnLvnItemchangedListView(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
            LRESULT OnNMCustomdrawListView(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
            LRESULT OnBnClickedButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnBnClickedButtonDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnBnClickedButtonNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnBnClickedButtonCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnBnClickedButtonDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnCbnSelchangeComboEncode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnCbnSelchangeComboMethod(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnBnClickedCheckClickable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnEnChangeEditRegex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnEnChangeEditTitle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnEnChangeEditShortcut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
            LRESULT OnEnChangeEditUrl(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

            void save() const;

        private:

            /// presetidしかないからクラスにする必要ないけど・・
            struct ItemData
            {
                int presetid;
            };

            CListViewCtrl listView_;
            std::list<ItemData> itemDataBuffer_;

            BackgroundColor backgroundColor_;

            CButton upButton_;
            CButton downButton_;
            CButton newButton_;
            CButton copyButton_;
            CButton deleteButton_;

            CEdit titleEdit_;
            CEdit shortcutEdit_;
            CEdit urlEdit_;
            CComboBox encodeComboBox_;
            CComboBox methodComboBox_;
            CEdit regexEdit_;
            CButton clickableCheckbox_;

            CButton tripleClickCheckBox_;
            CButton ctrlDoubleClickCheckBox_;
            CButton ctrlTripleClickCheckBox_;

            CButton warningOpenUrlCheckBox_;

            std::vector<std::unique_ptr<Column>> columnList_;

        private:
            LRESULT moveItem(int dragIndex, int dropIndex);
            void selectByIndex(int index);

            DISALLOW_COPY_AND_ASSIGN(SearchSiteForm);
        public:
            LRESULT OnNMDblclkListView(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
        };
    }
}

#endif // BKZ_SETTING_SEARCH_SITE_H
