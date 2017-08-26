/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_DETECT_BECKY_NEW_VERSION_DLG_H
#define BKZ_DETECT_BECKY_NEW_VERSION_DLG_H

#include "resource.h"
#include "wtlheader.h"
#include "defs.h"

namespace bkzoo
{
    namespace setting
    {

        class DetectBeckyNewVersionDlg final : public CDialogImpl<DetectBeckyNewVersionDlg>
        {
        public:
            enum { IDD = IDD_DETECT_NEW_BECKY_VERSION_DLG };

            BEGIN_MSG_MAP(DetectBeckyNewVersionDlg)
                MSG_WM_INITDIALOG(OnInitDialog)
                MSG_WM_TIMER(OnTimer)
                COMMAND_ID_HANDLER_EX(IDOK, OnOK)
                COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
                NOTIFY_HANDLER(IDC_SYSLINK_BECKY_DOWNLOAD, NM_CLICK, OnNMClickSyslinkBeckyDownload)
                COMMAND_HANDLER(IDC_CHECK_CHECK_BECKY_UPDATE_DISABLE, BN_CLICKED, OnBnClickedCheckCheckBeckyUpdateDisable)
            END_MSG_MAP()

            BOOL OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */);
            void OnTimer(UINT_PTR nIDEvent);
            void OnOK(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */);
            void OnCancel(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */);
            LRESULT OnNMClickSyslinkBeckyDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
            LRESULT OnBnClickedCheckCheckBeckyUpdateDisable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

        public:
            DetectBeckyNewVersionDlg() = default;
            ~DetectBeckyNewVersionDlg() = default;

        private:
            void displayCountDownTimeOnOkButton();
            void closeDlg(int nID);

            static const UINT ID_BECKY_UPDATE_CHECK_DLG_AUTO_CLOSE_TIMER = 1000;
            static const UINT ID_COUNT_DOWN_TIMER = 1001;
            static const int CountDownTime_msec = 8000; // 初期値8s

            int elapsedTime_sec_ = 0; // 経過時間(s)
            CButton okButton_;
            CStatic messageStatic_;
            CButton checkBeckyUpdateDisableCheckbox_;

            DISALLOW_COPY_AND_ASSIGN(DetectBeckyNewVersionDlg);
        };

    }
}

#endif // BKZ_DETECT_BECKY_NEW_VERSION_DLG_H
