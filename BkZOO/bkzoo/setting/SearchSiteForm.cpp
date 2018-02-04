/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "SearchSiteForm.h"

#include "ColumnImpl.h"
#include "util/RegexSearcher.h"
#include "util/GetResourceString.h"
#include "util/RegexSearcher.h"
#include "registry\Registry.h"

#include "bkzoo_string.h"
#include "bkzoo_config.h"

#include <regex>

#include "for_debug/detect_memory_leak.h"

LRESULT bkzoo::setting::SearchSiteForm::OnCtlColorDlg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return reinterpret_cast<LRESULT>(backgroundColor_.hbrush());
}

LRESULT bkzoo::setting::SearchSiteForm::OnCtlColorStatic(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
    if ((HWND)lParam == GetDlgItem(IDC_STATIC_Regex_Error))
    {
        // エラーメッセージの文字色を赤色にする。
        SetBkMode((HDC)wParam, TRANSPARENT);
        SetTextColor((HDC)wParam, RGB(255, 0, 0));
    }

    return reinterpret_cast<LRESULT>(backgroundColor_.hbrush());
}

BOOL bkzoo::setting::SearchSiteForm::OnInitDialog(CWindow /* wndFocus */, LPARAM /* lInitParam */)
{
    // 各種コントロール取得
    listView_ = GetDlgItem(IDC_LIST_VIEW);
    upButton_ = GetDlgItem(IDC_BUTTON_UP);
    downButton_ = GetDlgItem(IDC_BUTTON_DOWN);
    newButton_ = GetDlgItem(IDC_BUTTON_NEW);
    copyButton_ = GetDlgItem(IDC_BUTTON_COPY);
    deleteButton_ = GetDlgItem(IDC_BUTTON_DELETE);

    titleEdit_ = GetDlgItem(IDC_EDIT_TITLE);
    shortcutEdit_ = GetDlgItem(IDC_EDIT_SHORTCUT);
    urlEdit_ = GetDlgItem(IDC_EDIT_URL);
    encodeComboBox_ = GetDlgItem(IDC_COMBO_ENCODE);
    methodComboBox_ = GetDlgItem(IDC_COMBO_METHOD);
    regexEdit_ = GetDlgItem(IDC_EDIT_REGEX);
    clickableCheckbox_ = GetDlgItem(IDC_CHECK_CLICKABLE);

    // encodeComboBox_初期化
    encodeComboBox_.AddString(L"UTF-8");
    encodeComboBox_.AddString(L"Shift_JIS");
    encodeComboBox_.AddString(L"EUC-JP");

    // methodComboBox_初期化
    methodComboBox_.AddString(L"GET");
    methodComboBox_.AddString(L"POST");

    // shortcutEdit_初期化
    shortcutEdit_.SetLimitText(1);

    // listView_初期化
    listView_.SetExtendedListViewStyle(LVS_EX_CHECKBOXES | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);// | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
    listView_.InsertColumn(ColumnIndex::OnOff, L"On/Off", LVCFMT_LEFT, 48);
    listView_.InsertColumn(ColumnIndex::Preset, L"ReadOnly", LVCFMT_LEFT, 61);
    listView_.InsertColumn(ColumnIndex::Title, L"Title", LVCFMT_LEFT, 114);
    listView_.InsertColumn(ColumnIndex::Shortcut, L"Shortcut", LVCFMT_LEFT, 55);
    listView_.InsertColumn(ColumnIndex::URL, L"URL", LVCFMT_LEFT, 99);
    listView_.InsertColumn(ColumnIndex::Encode, L"Encode", LVCFMT_LEFT, 57);
    listView_.InsertColumn(ColumnIndex::Method, L"Method", LVCFMT_LEFT, 49);
    listView_.InsertColumn(ColumnIndex::Regex, L"Regex", LVCFMT_LEFT, 50);
    listView_.InsertColumn(ColumnIndex::Clickable, L"MouseClick", LVCFMT_LEFT, 71);

    const auto& sites = Config::instance().sites();
    size_t i = 0;
    for (const auto& site : sites)
    {
        listView_.AddItem(i, ColumnIndex::OnOff, L"");
        listView_.AddItem(i, ColumnIndex::Preset, site.preset_bool() ? L"readonly" : L"editable");
        listView_.AddItem(i, ColumnIndex::Title, site.title().c_str());
        listView_.AddItem(i, ColumnIndex::Shortcut, site.shortcut().c_str());
        listView_.AddItem(i, ColumnIndex::URL, site.url().c_str());
        listView_.AddItem(i, ColumnIndex::Encode, site.encode().c_str());
        listView_.AddItem(i, ColumnIndex::Method, site.method().c_str());
        listView_.AddItem(i, ColumnIndex::Regex, site.regex().c_str());
        listView_.AddItem(i, ColumnIndex::Clickable, site.clickable().c_str());
        const BOOL enabled = (site.enabled() == L"true");
        listView_.SetCheckState(i, enabled);
        itemDataBuffer_.push_back(ItemData{ site.preset_bool() ? site.presetid_int() : -1 });
        listView_.SetItemData(i, reinterpret_cast<DWORD_PTR>(&itemDataBuffer_.back()));
        ++i;
    }

    // 各種コントロールの値をlistViewの各ColumnにセットするColumnオブジェクトを生成
    columnList_.push_back(std::make_unique<NullColumn>(listView_, ColumnIndex::OnOff));
    columnList_.push_back(std::make_unique<NullColumn>(listView_, ColumnIndex::Preset));
    columnList_.push_back(std::make_unique<EditColumn>(listView_, ColumnIndex::Title, titleEdit_));
    columnList_.push_back(std::make_unique<EditColumn>(listView_, ColumnIndex::Shortcut, shortcutEdit_));
    columnList_.push_back(std::make_unique<EditColumn>(listView_, ColumnIndex::URL, urlEdit_));
    columnList_.push_back(std::make_unique<ComboBoxColumn>(listView_, ColumnIndex::Encode, encodeComboBox_));
    columnList_.push_back(std::make_unique<ComboBoxColumn>(listView_, ColumnIndex::Method, methodComboBox_));
    columnList_.push_back(std::make_unique<EditColumn>(listView_, ColumnIndex::Regex, regexEdit_));
    columnList_.push_back(std::make_unique<CheckBoxColumn>(listView_, ColumnIndex::Clickable, clickableCheckbox_));

    // 上で実施している columnList_ 初期化の後ではないと以下を実行できない。
    // 以下で一番上の行を選択すると columnList_ が変更されるため。
    if (listView_.GetItemCount() > 0)
    {
        // 一番上を選択状態にする
        selectByIndex(0);
    }


    // マウスクリック用コントロール
    tripleClickCheckBox_ = GetDlgItem(IDC_CHECK_TRIPLE_CLICK_SERVER);
    ctrlDoubleClickCheckBox_ = GetDlgItem(IDC_CHECK_CTRL_DOUBLE_CLICK);
    ctrlTripleClickCheckBox_ = GetDlgItem(IDC_CHECK_CTRL_TRIPLE_CLICK);
    warningOpenUrlCheckBox_ = GetDlgItem(IDC_CHECK_WARNING_OPEN_URL);

    // tripleClickCheckbox_初期化
    {
        const int checked = Config::instance().tripleClick() ? BST_CHECKED : BST_UNCHECKED;
        tripleClickCheckBox_.SetCheck(checked);
    }

    // ctrlDoubleClickCheckbox_初期化
    {
        const int checked = Config::instance().ctrlDoubleClick() ? BST_CHECKED : BST_UNCHECKED;
        ctrlDoubleClickCheckBox_.SetCheck(checked);
    }

    // ctrlTripleClickCheckBox_初期化
    {
        const int checked = Config::instance().ctrlTripleClick() ? BST_CHECKED : BST_UNCHECKED;
        ctrlTripleClickCheckBox_.SetCheck(checked);
    }

    // warningOpenUrlCheckBox_初期化
    {
        // ここだけConfigではなくてRegistry
        const bool isSkipWarningOpenUrl = registry::Registry::isSkipWarningOpenUrl();
        warningOpenUrlCheckBox_.SetCheck(!isSkipWarningOpenUrl);
    }


    // ToolTip
    {
        CToolTipCtrl tooltip;
        tooltip.Create(m_hWnd);
        tooltip.Activate(TRUE);
        tooltip.SetMaxTipWidth(400);
        tooltip.SetDelayTime(TTDT_AUTOPOP, 30000);

        CToolInfo tiEdit(TTF_SUBCLASS, tripleClickCheckBox_, 0, NULL
            , _T("\\\\やfile:\\\\で始まる共有サーバー、およびhttpで始まるURLなどを開きます。\r\n"
                "マウスクリックで検索を有効にしたサイトで検索します。"));
        tooltip.AddTool(tiEdit);
    }
    {
        CToolTipCtrl tooltip;
        tooltip.Create(m_hWnd);
        tooltip.Activate(TRUE);
        tooltip.SetMaxTipWidth(4700);
        tooltip.SetDelayTime(TTDT_AUTOPOP, 30000);

        CToolInfo tiEdit(TTF_SUBCLASS, ctrlDoubleClickCheckBox_, 0, NULL
            , _T("マウスクリックで検索を有効にしたサイトのうち、正規表現でヒットしたサイトを上位から順に検索します。\r\n"
                "正規表現にヒットするサイトがない場合、Googleで検索します。\r\n"
                "クリップボード内のURLに <file://> を付加して貼り付けます。"));
        tooltip.AddTool(tiEdit);
    }
    {
        CToolTipCtrl tooltip;
        tooltip.Create(m_hWnd);
        tooltip.Activate(TRUE);
        tooltip.SetMaxTipWidth(4700);
        tooltip.SetDelayTime(TTDT_AUTOPOP, 30000);

        CToolInfo tiEdit(TTF_SUBCLASS, ctrlTripleClickCheckBox_, 0, NULL
            , _T("Ctrl + トリプルクリックして選択した文字列が\\\\で始まる共有サーバー、またはhttpで始まるURLなどに、\r\n"
                "<file://> や <> を付加して貼り付けます。"));
        tooltip.AddTool(tiEdit);
    }

    return TRUE;
}


LRESULT bkzoo::setting::SearchSiteForm::OnNMCustomdrawListView(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    switch (pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
    {
        return CDRF_NOTIFYITEMDRAW;
    }
    case CDDS_ITEMPREPAINT:
    {
        // 選択行の背景色は常にシステムカラーで表示する。
        // LVS_SHOWSELALWAYS でないときの選択行は if (pLVCD->nmcd.uItemState & CDIS_SELECTED) で
        // 取得するが、LVS_SHOWSELALWAYS のときの選択行の取得方法は下記。
        UINT uiState = listView_.GetItemState(pLVCD->nmcd.dwItemSpec, LVIS_SELECTED);
        if (uiState & LVIS_SELECTED)
        {
            pLVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
            pLVCD->nmcd.uItemState &= ~CDIS_SELECTED;
        }

        // チェックがない場合はDisableに見せるために文字色をグレーにする。
        if (!listView_.GetCheckState(pLVCD->nmcd.dwItemSpec))
        {
            pLVCD->clrText = RGB(0x88, 0x88, 0x88);
        }
        return CDRF_NEWFONT;
    }
    default:
    {
        // do nothing
        return 0;
    }
    }
}


LRESULT bkzoo::setting::SearchSiteForm::OnLvnItemchangedListView(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    const int rowIndex = pNMLV->iItem;

    // 選択行が変更されたときの処理
    const bool selected = (pNMLV->uChanged == LVIF_STATE) && (pNMLV->uNewState == (LVFIS_FOCUSED | LVIS_SELECTED));
    if (selected)
    {
        wchar_t buffer[_MAX_PATH] = { 0 };

        // Preset
        listView_.GetItemText(rowIndex, ColumnIndex::Preset, buffer, _MAX_PATH);
        const std::wstring presetStr(buffer);
        const BOOL isCustom = (presetStr == L"readonly") ? FALSE : TRUE;

        // Delete Button
        deleteButton_.EnableWindow(isCustom);

        // Title
        listView_.GetItemText(rowIndex, ColumnIndex::Title, buffer, _MAX_PATH);
        titleEdit_.SetWindowText(buffer);
        titleEdit_.EnableWindow(isCustom);

        // Shortcut
        listView_.GetItemText(rowIndex, ColumnIndex::Shortcut, buffer, _MAX_PATH);
        shortcutEdit_.SetWindowText(buffer);
        shortcutEdit_.EnableWindow(isCustom);

        // URL
        listView_.GetItemText(rowIndex, ColumnIndex::URL, buffer, _MAX_PATH);
        urlEdit_.SetWindowText(buffer);
        urlEdit_.EnableWindow(isCustom);

        // Encode
        listView_.GetItemText(rowIndex, ColumnIndex::Encode, buffer, _MAX_PATH);
        int error = encodeComboBox_.SelectString(0, buffer);
        if (error == CB_ERR)
        {
            // 存在しないencodeの場合は最初のitemを選択
            if (encodeComboBox_.GetCount() > 0)
            {
                encodeComboBox_.SetCurSel(0);
            }
        }
        encodeComboBox_.EnableWindow(isCustom);

        // Method
        listView_.GetItemText(rowIndex, ColumnIndex::Method, buffer, _MAX_PATH);
        error = methodComboBox_.SelectString(0, buffer);
        if (error == CB_ERR)
        {
            // 存在しないmethodの場合は最初のitemを選択
            if (methodComboBox_.GetCount() > 0)
            {
                methodComboBox_.SetCurSel(0);
            }
        }
        methodComboBox_.EnableWindow(isCustom);

        // Regex
        listView_.GetItemText(rowIndex, ColumnIndex::Regex, buffer, _MAX_PATH);
        regexEdit_.SetWindowText(buffer);
        regexEdit_.EnableWindow(isCustom);
        const std::wstring regexString(buffer);

        // Clickable
        listView_.GetItemText(rowIndex, ColumnIndex::Clickable, buffer, _MAX_PATH);
        const std::wstring clickableStr(buffer);
        const int check = ((clickableStr == L"true") && !regexString.empty()) ? BST_CHECKED : BST_UNCHECKED;
        clickableCheckbox_.SetCheck(check);
        clickableCheckbox_.EnableWindow(isCustom && !regexString.empty() && util::RegexUtil::isValidRegex(regexString));
    }

    return 0;
}


LRESULT bkzoo::setting::SearchSiteForm::OnCbnSelchangeComboEncode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    columnList_.at(ColumnIndex::Encode)->setItemText(listView_.GetSelectedIndex());
    return 0;
}
LRESULT bkzoo::setting::SearchSiteForm::OnCbnSelchangeComboMethod(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    columnList_.at(ColumnIndex::Method)->setItemText(listView_.GetSelectedIndex());
    return 0;
}
LRESULT bkzoo::setting::SearchSiteForm::OnBnClickedCheckClickable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    columnList_.at(ColumnIndex::Clickable)->setItemText(listView_.GetSelectedIndex());
    return 0;
}
LRESULT bkzoo::setting::SearchSiteForm::OnEnChangeEditRegex(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // 不正な正規表現なら警告を出す
    {
        wchar_t regexString[_MAX_PATH] = { 0 };
        regexEdit_.GetWindowText(regexString, _MAX_PATH);
        std::wstring regexStr(regexString);
        string::StringUtils::trim(regexStr);
        CStatic regexError = GetDlgItem(IDC_STATIC_Regex_Error);
        if (regexStr.empty())
        {
            clickableCheckbox_.EnableWindow(FALSE);
            regexError.SetWindowText(L"");
        }
        else if (util::RegexUtil::isValidRegex(regexString))
        {
            clickableCheckbox_.EnableWindow(TRUE);
            regexError.SetWindowText(L"");
        }
        else
        {
            clickableCheckbox_.EnableWindow(FALSE);
            regexError.SetWindowText(util::getResourceStringW(IDS_STRING_WARNING_MESSAGE_REGEX).c_str());
        }

        columnList_.at(ColumnIndex::Regex)->setItemText(listView_.GetSelectedIndex());

        return 0;
    }
}
LRESULT bkzoo::setting::SearchSiteForm::OnEnChangeEditTitle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    columnList_.at(ColumnIndex::Title)->setItemText(listView_.GetSelectedIndex());

    return 0;
}
LRESULT bkzoo::setting::SearchSiteForm::OnEnChangeEditShortcut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    columnList_.at(ColumnIndex::Shortcut)->setItemText(listView_.GetSelectedIndex());

    return 0;
}
LRESULT bkzoo::setting::SearchSiteForm::OnEnChangeEditUrl(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    columnList_.at(ColumnIndex::URL)->setItemText(listView_.GetSelectedIndex());

    return 0;
}


LRESULT bkzoo::setting::SearchSiteForm::moveItem(const int dragIndex, const int dropIndex)
{
    // 移動元を保存
    const DWORD_PTR ptr = listView_.GetItemData(dragIndex);
    const ItemData* itemData = reinterpret_cast<ItemData*>(ptr);
    wchar_t buffer[_MAX_PATH] = { 0 };
    listView_.GetItemText(dragIndex, ColumnIndex::Preset, buffer, _MAX_PATH);
    const std::wstring preset(buffer);
    listView_.GetItemText(dragIndex, ColumnIndex::Title, buffer, _MAX_PATH);
    const std::wstring title(buffer);
    listView_.GetItemText(dragIndex, ColumnIndex::Shortcut, buffer, _MAX_PATH);
    const std::wstring shortcut(buffer);
    listView_.GetItemText(dragIndex, ColumnIndex::URL, buffer, _MAX_PATH);
    const std::wstring url(buffer);
    listView_.GetItemText(dragIndex, ColumnIndex::Encode, buffer, _MAX_PATH);
    const std::wstring encode(buffer);
    listView_.GetItemText(dragIndex, ColumnIndex::Method, buffer, _MAX_PATH);
    const std::wstring method(buffer);
    listView_.GetItemText(dragIndex, ColumnIndex::Regex, buffer, _MAX_PATH);
    const std::wstring regex(buffer);
    listView_.GetItemText(dragIndex, ColumnIndex::Clickable, buffer, _MAX_PATH);
    const std::wstring clickable(buffer);
    const BOOL checked = listView_.GetCheckState(dragIndex);

    // 移動元を削除
    listView_.DeleteItem(dragIndex);

    // 保存していた移動元を移動先に挿入
    listView_.AddItem(dropIndex, ColumnIndex::OnOff, L"");
    listView_.AddItem(dropIndex, ColumnIndex::Preset, preset.c_str());
    listView_.AddItem(dropIndex, ColumnIndex::Title, title.c_str());
    listView_.AddItem(dropIndex, ColumnIndex::Shortcut, shortcut.c_str());
    listView_.AddItem(dropIndex, ColumnIndex::URL, url.c_str());
    listView_.AddItem(dropIndex, ColumnIndex::Encode, encode.c_str());
    listView_.AddItem(dropIndex, ColumnIndex::Method, method.c_str());
    listView_.AddItem(dropIndex, ColumnIndex::Regex, regex.c_str());
    listView_.AddItem(dropIndex, ColumnIndex::Clickable, clickable.c_str());
    listView_.SetCheckState(dropIndex, checked);
    listView_.SetItemData(dropIndex, reinterpret_cast<DWORD_PTR>(itemData));

    // 移動先を選択状態にする
    selectByIndex(dropIndex);

    return 0;
}

void bkzoo::setting::SearchSiteForm::selectByIndex(int index)
{
    listView_.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}

LRESULT bkzoo::setting::SearchSiteForm::OnBnClickedButtonUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // dragを開始したindex。つまり現在選択中のindex。
    int dragIndex = listView_.GetSelectedIndex();
    if (dragIndex == -1)
    {
        // 未選択
        return S_OK;
    }
    const int dropIndex = dragIndex - 1; // 一つ上にdrop。つまり移動先のindex。

    if (dragIndex == 0)
    {
        // 一番上なので何もしない
        return S_OK;
    }
    if (dragIndex >= listView_.GetItemCount())
    {
        // 不正な値なので何もしない
        assert(false);
        return S_OK;
    }

    // dragIndexをdropIndexに移動。
    moveItem(dragIndex, dropIndex);

    return 0;
}


LRESULT bkzoo::setting::SearchSiteForm::OnBnClickedButtonDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // dragを開始したindex。つまり現在選択中のindex。
    int dragIndex = listView_.GetSelectedIndex();
    if (dragIndex == -1)
    {
        // 未選択
        return S_OK;
    }
    const int dropIndex = dragIndex + 1; // 一つ下にdrop。つまり移動先のindex。

    if (dragIndex >= (listView_.GetItemCount() - 1))
    {
        // 最後尾（または不正な値）なので何もしない
        assert(dragIndex == listView_.GetItemCount() - 1 ? true : false);
        return S_OK;
    }

    // dragIndexをdropIndexに移動。
    moveItem(dragIndex, dropIndex);

    return 0;
}

LRESULT bkzoo::setting::SearchSiteForm::OnBnClickedButtonNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    int newIndex = listView_.GetSelectedIndex();
    if (newIndex == -1)
    {
        newIndex = 0;
    }

    itemDataBuffer_.push_back(ItemData{ -1 });
    listView_.AddItem(newIndex, ColumnIndex::OnOff, L"");
    listView_.AddItem(newIndex, ColumnIndex::Preset, L"editable");
    listView_.AddItem(newIndex, ColumnIndex::Title, L"");
    listView_.AddItem(newIndex, ColumnIndex::Shortcut, L"");
    listView_.AddItem(newIndex, ColumnIndex::URL, L"");
    listView_.AddItem(newIndex, ColumnIndex::Encode, L"UTF-8");
    listView_.AddItem(newIndex, ColumnIndex::Method, L"GET");
    listView_.AddItem(newIndex, ColumnIndex::Regex, L"");
    listView_.AddItem(newIndex, ColumnIndex::Clickable, L"false");
    listView_.SetCheckState(newIndex, TRUE);
    listView_.SetItemData(newIndex, reinterpret_cast<DWORD_PTR>(&itemDataBuffer_.back()));

    // 新規追加したら選択状態にする
    selectByIndex(newIndex);

    return 0;
}


LRESULT bkzoo::setting::SearchSiteForm::OnBnClickedButtonCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    const int selectedIndex = listView_.GetSelectedIndex();
    if (selectedIndex == -1)
    {
        return 0;
    }

    wchar_t buffer[_MAX_PATH] = { 0 };
    //listView_.GetItemText(selectedIndex, ColumnIndex::Preset, buffer, _MAX_PATH);
    //const std::wstring preset(buffer);
    listView_.GetItemText(selectedIndex, ColumnIndex::Title, buffer, _MAX_PATH);
    const std::wstring title(buffer);
    listView_.GetItemText(selectedIndex, ColumnIndex::Shortcut, buffer, _MAX_PATH);
    const std::wstring shortcut(buffer);
    listView_.GetItemText(selectedIndex, ColumnIndex::URL, buffer, _MAX_PATH);
    const std::wstring url(buffer);
    listView_.GetItemText(selectedIndex, ColumnIndex::Encode, buffer, _MAX_PATH);
    const std::wstring encode(buffer);
    listView_.GetItemText(selectedIndex, ColumnIndex::Method, buffer, _MAX_PATH);
    const std::wstring method(buffer);
    listView_.GetItemText(selectedIndex, ColumnIndex::Regex, buffer, _MAX_PATH);
    const std::wstring regex(buffer);
    listView_.GetItemText(selectedIndex, ColumnIndex::Clickable, buffer, _MAX_PATH);
    const std::wstring clickable(buffer);
    const BOOL checked = listView_.GetCheckState(selectedIndex);


    listView_.AddItem(selectedIndex, ColumnIndex::OnOff, L"");
    listView_.AddItem(selectedIndex, ColumnIndex::Preset, L"editable");
    const std::wstring copyMenu(L"copy_" + title);
    listView_.AddItem(selectedIndex, ColumnIndex::Title, copyMenu.c_str());
    listView_.AddItem(selectedIndex, ColumnIndex::Shortcut, shortcut.c_str());
    listView_.AddItem(selectedIndex, ColumnIndex::URL, url.c_str());
    listView_.AddItem(selectedIndex, ColumnIndex::Encode, encode.c_str());
    listView_.AddItem(selectedIndex, ColumnIndex::Method, method.c_str());
    listView_.AddItem(selectedIndex, ColumnIndex::Regex, regex.c_str());
    listView_.AddItem(selectedIndex, ColumnIndex::Clickable, clickable.c_str());
    listView_.SetCheckState(selectedIndex, checked);
    itemDataBuffer_.push_back(ItemData{ -1 });
    listView_.SetItemData(selectedIndex, reinterpret_cast<DWORD_PTR>(&itemDataBuffer_.back()));

    // コピー先を選択状態にする
    selectByIndex(selectedIndex);

    return 0;
}


LRESULT bkzoo::setting::SearchSiteForm::OnBnClickedButtonDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    const int selectedIndex = listView_.GetSelectedIndex();
    if (selectedIndex == -1)
    {
        return 0;
    }

    listView_.DeleteItem(selectedIndex);


    //
    // 削除後に同じ行を選択状態にする
    //

    if (listView_.GetItemCount() == 0)
    {
        // リストが空なら選択せずに抜ける
        return 0;
    }

    // 新しい選択行は削除した行と同じがデフォルト。
    int newIndex = selectedIndex;

    if (selectedIndex == listView_.GetItemCount())
    {
        // 最後の行を削除した場合は、一つ上を選択状態にする
        --newIndex;
    }

    // 上で決めた行を選択状態にする
    selectByIndex(newIndex);

    return 0;
}

void bkzoo::setting::SearchSiteForm::save() const
{
    SiteList sites;
    wchar_t buffer[_MAX_PATH] = { 0 };
    const int count = listView_.GetItemCount();
    for (int i = 0; i < count; ++i)
    {
        // 保存の度に作り直す
        Site site;

        // for presetid & preset
        const DWORD_PTR ptr = listView_.GetItemData(i);
        const ItemData* itemData = reinterpret_cast<ItemData*>(ptr);

        // presetid
        site.setPresetid(std::to_wstring(itemData->presetid));

        // preset
        listView_.GetItemText(i, ColumnIndex::Preset, buffer, _MAX_PATH);
        const std::wstring preset(buffer);
        site.setPreset(preset == L"readonly" ? L"true" : L"false");

        // title
        listView_.GetItemText(i, ColumnIndex::Title, buffer, _MAX_PATH);
        site.setTitle(buffer);

        // shortcut
        listView_.GetItemText(i, ColumnIndex::Shortcut, buffer, _MAX_PATH);
        site.setShortcut(buffer);

        // url
        listView_.GetItemText(i, ColumnIndex::URL, buffer, _MAX_PATH);
        site.setUrl(buffer);

        // encode
        listView_.GetItemText(i, ColumnIndex::Encode, buffer, _MAX_PATH);
        site.setEncode(buffer);

        // method
        listView_.GetItemText(i, ColumnIndex::Method, buffer, _MAX_PATH);
        site.setMethod(buffer);

        // regex
        listView_.GetItemText(i, ColumnIndex::Regex, buffer, _MAX_PATH);
        std::wstring regexString(buffer);
        string::StringUtils::trim(regexString);
        site.setRegex(regexString);

        // clickable
        listView_.GetItemText(i, ColumnIndex::Clickable, buffer, _MAX_PATH);
        site.setClickable(buffer);
        if (regexString.empty())
            site.setClickable(L"false"); // 正規表現が空なら強制的にfalse

        // checked
        const BOOL checked = listView_.GetCheckState(i);
        site.setEnabled(checked ? L"true" : L"false");

        // siteを追加
        sites.push_back(site);
    }

    Config::instance().setSites(sites);


    // tripleClickCheckbox_保存
    {
        const bool enabled = (tripleClickCheckBox_.GetCheck() == BST_CHECKED) ? true : false;
        Config::instance().setTripleClick(enabled);
    }

    // ctrlDoubleClickCheckbox_保存
    {
        const bool enabled = (ctrlDoubleClickCheckBox_.GetCheck() == BST_CHECKED) ? true : false;
        Config::instance().setCtrlDoubleClick(enabled);
    }

    // ctrlTripleClickCheckbox_保存
    {
        const bool enabled = (ctrlTripleClickCheckBox_.GetCheck() == BST_CHECKED) ? true : false;
        Config::instance().setCtrlTripleClick(enabled);
    }

    // warningOpenUrlCheckBox_保存
    {
        const bool enabled = (warningOpenUrlCheckBox_.GetCheck() == BST_CHECKED) ? true : false;
        if (enabled)
        {
            registry::Registry::removeIgnoreFlagForWarningOpenUrl();
        }
        else
        {
            registry::Registry::addIgnoreFlagForWarningOpenUrl();
        }
    }
}


LRESULT bkzoo::setting::SearchSiteForm::OnNMDblclkListView(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    const int rowIndex = pNMLV->iItem;
    const BOOL checked = listView_.GetCheckState(rowIndex);

    listView_.SetCheckState(rowIndex, (checked == FALSE) ? TRUE : FALSE);

    return 0;
}
