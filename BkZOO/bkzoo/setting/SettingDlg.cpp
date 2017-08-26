/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "SettingDlg.h"

#include "ColumnImpl.h"
#include "config/ConfigRepository.h"

#include "bkzoo_config.h"

#include <shellapi.h>

#include "debug/detect_memory_leak.h"

void bkzoo::setting::SettingDlg::OnOK(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */)
{
    // Configに保存
    searchSiteForm_.save();
    checkBeckyUpdateForm_.save();

    // Configをiniファイルに保存
    ConfigRepository::instance().saveConfig(Config::instance());

    EndDialog(nID);
}


void bkzoo::setting::SettingDlg::OnCancel(UINT /* uNotifyCode */, int nID, CWindow /* wndCtl */)
{
    EndDialog(nID);
}


BOOL bkzoo::setting::SettingDlg::OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */)
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

    // tab
    {
        tab_ = GetDlgItem(IDC_MAIN_TAB);

        // 各FormViewを生成して追加
        searchSiteForm_.Create(tab_.m_hWnd);
        tab_.AddItem(TCIF_TEXT | TCIF_PARAM, L"検索サイト", 0, reinterpret_cast<int>(searchSiteForm_.m_hWnd));
        checkBeckyUpdateForm_.Create(tab_.m_hWnd);
        tab_.AddItem(TCIF_TEXT | TCIF_PARAM, L"更新チェック", 0, reinterpret_cast<int>(checkBeckyUpdateForm_.m_hWnd));

        // 各FormViewの位置合わせ。tab_.AddItem()の後に以下を実施しないとAdjustRect()がうまく動作しない。
        CRect rect;
        tab_.GetClientRect(rect);
        tab_.AdjustRect(FALSE, rect); // ここでtabタイトルの下の矩形を取得

        searchSiteForm_.MoveWindow(rect);
        checkBeckyUpdateForm_.MoveWindow(rect);

        // 前回の最終選択が何かにかかわらず最初のタブを選択
        tab_.SetCurSel(0);
        switchWindow(true);
    }

    return TRUE;
}



LRESULT bkzoo::setting::SettingDlg::OnTabSelChange(int /*idCtrl*/, LPNMHDR /*pNMHDR*/, BOOL& /*bHandled*/)
{
    switchWindow(true);
    return 0;
}
LRESULT bkzoo::setting::SettingDlg::OnTabSelChanging(int /*idCtrl*/, LPNMHDR /*pNMHDR*/, BOOL& /*bHandled*/)
{
    switchWindow(false);
    return 0;
}


void bkzoo::setting::SettingDlg::switchWindow(bool bShow)
{
    int nIndex = tab_.GetCurSel();
    TCITEM ti;
    ti.mask = TCIF_PARAM;
    tab_.GetItem(nIndex, &ti);
    CWindow win = (HWND)ti.lParam;
    win.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
    if (bShow)
    {
        win.SetFocus();
    }
}


LRESULT bkzoo::setting::SettingDlg::OnNMClickSyslinkBkzooHome(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    PNMLINK pNMLink = (PNMLINK)pNMHDR;
    ::ShellExecute(m_hWnd, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);

    return 0;
}
