/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "DetectBeckyNewVersionDlg.h"

#include "util/BeckyVersion.h"

#include "bkzoo_config.h"
#include "bkzoo_string.h"

#include <BeckyAPI.h>
extern CBeckyAPI beckyApi;

#include <shellapi.h>

#include <sstream>

#include "for_debug/detect_memory_leak.h"


BOOL bkzoo::setting::DetectBeckyNewVersionDlg::OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */)
{
    // スクリーンの中央に配置
    CenterWindow();

    // 大きいアイコン設定
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR,
        ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);

    // 小さいアイコン設定
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR,
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);

    // この Dialog を自動で閉じるための Timer をセット
    SetTimer(ID_BECKY_UPDATE_CHECK_DLG_AUTO_CLOSE_TIMER, CountDownTime_msec, NULL);

    // この Dialog を閉じるときのカウントダウン用の Timer をセット
    SetTimer(ID_COUNT_DOWN_TIMER, 1000/*1秒*/, NULL);

    // OK Button のテキスト変更のために取得
    okButton_ = GetDlgItem(IDOK);
    displayCountDownTimeOnOkButton();

    // ダイアログ上のメッセージにバージョン番号を埋め込むためにテキストを取得
    util::BeckyVersion beckyVersionLatest(util::BeckyVersion::getBeckyVersionLatest(beckyApi.GetVersion()));
    util::BeckyVersion beckyVersionOfMyself(util::BeckyVersion::getBeckyVersionOfMyself(beckyApi.GetVersion()));

    std::wostringstream message;
    message << L"新しいBecky!が見つかっています。\n    Ver." << wm_cast<std::wstring>(beckyVersionLatest.version())
        << L" （現バージョン " << wm_cast<std::wstring>(beckyVersionOfMyself.version()) << L"）";
    messageStatic_ = GetDlgItem(IDC_STATIC_DETECT_NEW_BECKY_VERSION);
    messageStatic_.SetWindowTextW(message.str().c_str());

    // Becky!更新の通知をやめるチェックボックスを取得
    checkBeckyUpdateDisableCheckbox_ = GetDlgItem(IDC_CHECK_CHECK_BECKY_UPDATE_DISABLE);

    return TRUE;
}


void bkzoo::setting::DetectBeckyNewVersionDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == ID_BECKY_UPDATE_CHECK_DLG_AUTO_CLOSE_TIMER)
    {
        closeDlg(IDOK);
    }
    else if (nIDEvent == ID_COUNT_DOWN_TIMER)
    {
        elapsedTime_sec_ += 1; // 1sec 経過
        displayCountDownTimeOnOkButton();
    }
    else
    {
        // do nothing
    }
}


void bkzoo::setting::DetectBeckyNewVersionDlg::OnOK(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */)
{
    closeDlg(nID);
}


void bkzoo::setting::DetectBeckyNewVersionDlg::OnCancel(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */)
{
    closeDlg(nID);
}


LRESULT bkzoo::setting::DetectBeckyNewVersionDlg::OnNMClickSyslinkBeckyDownload(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    PNMLINK pNMLink = (PNMLINK)pNMHDR;
    ::ShellExecute(m_hWnd, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);

    return 0;
}


void bkzoo::setting::DetectBeckyNewVersionDlg::displayCountDownTimeOnOkButton()
{
    int countDownTime_sec = CountDownTime_msec / 1000 - elapsedTime_sec_;
    std::wostringstream woss;
    woss << L"閉じる (" << countDownTime_sec << L" sec)";
    okButton_.SetWindowTextW(woss.str().c_str());
}


void bkzoo::setting::DetectBeckyNewVersionDlg::closeDlg(int nID)
{
    // 各種 Timer を close
    KillTimer(ID_BECKY_UPDATE_CHECK_DLG_AUTO_CLOSE_TIMER);
    KillTimer(ID_COUNT_DOWN_TIMER);

    // Dialog を close
    EndDialog(nID);
}


LRESULT bkzoo::setting::DetectBeckyNewVersionDlg::OnBnClickedCheckCheckBeckyUpdateDisable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    config::Config::instance().setCheckBeckyUpdate(checkBeckyUpdateDisableCheckbox_.GetCheck() != BST_CHECKED);

    return 0;
}
