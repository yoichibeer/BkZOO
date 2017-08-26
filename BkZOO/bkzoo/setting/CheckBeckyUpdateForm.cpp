/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "CheckBeckyUpdateForm.h"

#include "ColumnImpl.h"
#include "util/BeckyVersion.h"

#include "bkzoo_config.h"
#include "bkzoo_string.h"

#include <BeckyAPI.h>
extern CBeckyAPI beckyApi;

#include <shellapi.h>

#include "debug/detect_memory_leak.h"

BOOL bkzoo::setting::CheckBeckyUpdateForm::OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */)
{
    const bool checkBeckyUpdate = Config::instance().checkBeckyUpdate();
    // checkbox
    checkBeckyUpdateCheckbox_ = GetDlgItem(IDC_CHECK_BECKY_UPDATE);
    checkBeckyUpdateCheckbox_.SetCheck(checkBeckyUpdate ? BST_CHECKED : BST_UNCHECKED);

    // message
    messageStatic_ = GetDlgItem(IDC_STATIC_DETECT_NEW_BECKY_VERSION);
    util::BeckyVersion beckyVersionLatest(util::BeckyVersion::getBeckyVersionLatest(beckyApi.GetVersion()));
    util::BeckyVersion beckyVersionOfMyself(util::BeckyVersion::getBeckyVersionOfMyself(beckyApi.GetVersion()));
    std::wostringstream message;
    if (beckyVersionLatest > beckyVersionOfMyself)
    {
        message << L"新しいBecky!が見つかっています。\n    Ver." << wm_cast<std::wstring>(beckyVersionLatest.version())
            << L" （現バージョン " << wm_cast<std::wstring>(beckyVersionOfMyself.version()) << L"）";
    }
    else
    {
        message << L"新しいBecky!は見つかっていません。";
    }
    messageStatic_.SetWindowTextW(message.str().c_str());

    // groupbox
    groupbox_ = GetDlgItem(IDC_GROUP_BOX_CHECK_BECKY_UPDATE);

    // 機能説明
    descriptionStatic_ = GetDlgItem(IDC_STATIC_DESCRIPTION);

    // SysLink
//	beckyDownloadLinkCtrl_ = GetDlgItem(IDC_SYSLINK_BECKY_DOWNLOAD);

    // enable window
//	setEnable(checkBeckyUpdate);

    return TRUE;
}

LRESULT bkzoo::setting::CheckBeckyUpdateForm::OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return reinterpret_cast<LRESULT>(backgroundColor_.hbrush());
}

LRESULT bkzoo::setting::CheckBeckyUpdateForm::OnCtlColorStatic(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return reinterpret_cast<LRESULT>(backgroundColor_.hbrush());
}


//LRESULT bkzoo::setting::CheckBeckyUpdateForm::OnBnClickedCheckBeckyUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	setEnable(checkBeckyUpdateCheckbox_.GetCheck() == BST_CHECKED);
//
//	return 0;
//}

//void bkzoo::setting::CheckBeckyUpdateForm::setEnable(bool enable)
//{
////    const BOOL enableWindow = enable ? TRUE : FALSE;
//
////    groupbox_.EnableWindow(enableWindow);
////    descriptionStatic_.EnableWindow(enableWindow);
////    messageStatic_.EnableWindow(enableWindow);
////    beckyDownloadLinkCtrl_.EnableWindow(enable);
//}

LRESULT bkzoo::setting::CheckBeckyUpdateForm::OnNMClickSyslinkBeckyDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    PNMLINK pNMLink = (PNMLINK)pNMHDR;
    ::ShellExecute(m_hWnd, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);

    return 0;
}


void bkzoo::setting::CheckBeckyUpdateForm::save() const
{
    // checkBeckyUpdateCheckbox_保存
    {
        const bool checkBeckyUpdate = (checkBeckyUpdateCheckbox_.GetCheck() == BST_CHECKED);
        config::Config::instance().setCheckBeckyUpdate(checkBeckyUpdate);
    }
}
