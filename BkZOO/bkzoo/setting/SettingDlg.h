/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_SETTING_SETTING_DLG_H
#define BKZ_SETTING_SETTING_DLG_H

#include "Column.h"
#include "SearchSiteForm.h"
#include "CheckBeckyUpdateForm.h"

#include "resource.h"
#include "wtlheader.h"

#include <vector>
#include <memory>


namespace bkzoo
{
    namespace setting
    {

        class SettingDlg final : public CDialogImpl<SettingDlg>
        {
        public:
            enum { IDD = IDD_SETTING_DLG };

            BEGIN_MSG_MAP(SettingDlg)
                MSG_WM_INITDIALOG(OnInitDialog)
                COMMAND_ID_HANDLER_EX(IDOK, OnOK)
                COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
                NOTIFY_HANDLER(IDC_MAIN_TAB, TCN_SELCHANGE, OnTabSelChange)
                NOTIFY_HANDLER(IDC_MAIN_TAB, TCN_SELCHANGING, OnTabSelChanging)
                NOTIFY_HANDLER(IDC_SYSLINK_BKZOO_HOME, NM_CLICK, OnNMClickSyslinkBkzooHome)
            END_MSG_MAP()

        public:
            SettingDlg() = default;
            ~SettingDlg() = default;

            BOOL OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */);
            void OnOK(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */);
            void OnCancel(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */);
            LRESULT OnTabSelChange(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
            LRESULT OnTabSelChanging(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
            LRESULT OnNMClickSyslinkBkzooHome(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

        private:
            CTabCtrl tab_;

            SearchSiteForm searchSiteForm_;
            CheckBeckyUpdateForm checkBeckyUpdateForm_;

        private:
            void switchWindow(bool bShow);

            SettingDlg(const SettingDlg&) = delete;
            SettingDlg& operator=(const SettingDlg&) = delete;
        };

    }
}

#endif // BKZ_SETTING_SETTING_DLG_H
