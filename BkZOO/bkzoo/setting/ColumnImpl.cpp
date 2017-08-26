/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "ColumnImpl.h"

#include <string>

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace setting
    {
        NullColumn::NullColumn(CListViewCtrl& listViewCtrl, ColumnIndex columnIndex)
            : Column(listViewCtrl, columnIndex)
        {
        }

        void NullColumn::setItemText(int /*rowIndex*/)
        {
        }



        EditColumn::EditColumn(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex, const CEdit& edit)
            : Column(listViewCtrl, columnIndex), edit_(edit)
        {
        }

        void EditColumn::setItemText(int rowIndex)
        {
            wchar_t buffer[_MAX_PATH] = { 0 };
            listViewCtrl_.GetItemText(rowIndex, columnIndex_, buffer, _MAX_PATH);
            const std::wstring currentEdit(buffer);

            edit_.GetWindowText(buffer, _MAX_PATH);
            const std::wstring newEdit(buffer);

            if (currentEdit == newEdit)
                return; // 同じならスキップ

            listViewCtrl_.SetItemText(rowIndex, columnIndex_, buffer);
        }



        ComboBoxColumn::ComboBoxColumn(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex, const CComboBox& comboBox)
            : Column(listViewCtrl, columnIndex), comboBox_(comboBox)
        {
        }

        void ComboBoxColumn::setItemText(int rowIndex)
        {
            wchar_t buffer[_MAX_PATH] = { 0 };
            listViewCtrl_.GetItemText(rowIndex, columnIndex_, buffer, _MAX_PATH);
            const std::wstring currentCombo(buffer);

            comboBox_.GetWindowText(buffer, _MAX_PATH);
            const std::wstring newCombo(buffer);

            if (currentCombo == newCombo)
                return; // 同じならスキップ

            listViewCtrl_.SetItemText(rowIndex, columnIndex_, buffer);
        }



        CheckBoxColumn::CheckBoxColumn(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex, const CButton& checkBox)
            : Column(listViewCtrl, columnIndex), checkBox_(checkBox)
        {
        }

        void CheckBoxColumn::setItemText(int rowIndex)
        {
            wchar_t buffer[_MAX_PATH] = { 0 };
            listViewCtrl_.GetItemText(rowIndex, columnIndex_, buffer, _MAX_PATH);
            const std::wstring currentCheck(buffer);

            const std::wstring newCheck =
                (checkBox_.GetCheck() == BST_CHECKED) ? L"true" : L"false";

            if (currentCheck == newCheck)
                return; // 同じならスキップ

            listViewCtrl_.SetItemText(rowIndex, columnIndex_, newCheck.c_str());
        }

    }
}
