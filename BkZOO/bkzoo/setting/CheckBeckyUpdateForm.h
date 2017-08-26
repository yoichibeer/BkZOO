/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_SETTING_CHECK_BECKY_UPDATE_FORM_H
#define BKZ_SETTING_CHECK_BECKY_UPDATE_FORM_H

#include "Column.h"
#include "BackgroundColor.h"

#include "resource.h"
#include "wtlheader.h"

#include <vector>
#include <memory>

#include "defs.h"

namespace bkzoo
{
    namespace setting
    {

        class CheckBeckyUpdateForm : public CDialogImpl<CheckBeckyUpdateForm>
        {
        public:
            enum { IDD = IDD_CHECK_BECKY_UPDATE_FORMVIEW };

            BEGIN_MSG_MAP(CheckBeckyUpdateForm)
                MSG_WM_INITDIALOG(OnInitDialog)
                MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
                MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
                //				COMMAND_HANDLER(IDC_CHECK_BECKY_UPDATE, BN_CLICKED, OnBnClickedCheckBeckyUpdate)
                NOTIFY_HANDLER(IDC_SYSLINK_BECKY_DOWNLOAD, NM_CLICK, OnNMClickSyslinkBeckyDownload)
            END_MSG_MAP()

        public:
            CheckBeckyUpdateForm() = default;
            ~CheckBeckyUpdateForm() = default;

            // FormView初期化
            BOOL OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */);

            // FormView背景を白に変更
            LRESULT OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

            // FormView上のstaticコントロールの背景を白に変更
            LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

            // Becky!更新チェック機能の有効無効を設定
//			LRESULT OnBnClickedCheckBeckyUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

            // リンククリック
            LRESULT OnNMClickSyslinkBeckyDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

            // Configに保存
            void save() const;

        private:
            //			void setEnable(bool enable);

            BackgroundColor backgroundColor_;

            CButton checkBeckyUpdateCheckbox_;
            CStatic messageStatic_;
            CStatic groupbox_;
            CStatic descriptionStatic_;
            //CLinkCtrl beckyDownloadLinkCtrl_;

            DISALLOW_COPY_AND_ASSIGN(CheckBeckyUpdateForm);
        };

    }
}

#endif // BKZ_SETTING_CHECK_BECKY_UPDATE_FORM_H
