/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_SETTING_COLUMN_IMPL_H
#define BKZ_SETTING_COLUMN_IMPL_H

#include "Column.h"

namespace bkzoo
{
    namespace setting
    {

        class NullColumn : public Column
        {
        public:
            NullColumn(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex);
            virtual ~NullColumn() = default;

            void setItemText(int rowIndex);
        };



        class EditColumn : public Column
        {
        public:
            EditColumn(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex, const CEdit& edit);
            virtual ~EditColumn() = default;

            void setItemText(int rowIndex);

        private:
            const CEdit& edit_;
        };



        class ComboBoxColumn : public Column
        {
        public:
            ComboBoxColumn(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex, const CComboBox& comboBox);
            virtual ~ComboBoxColumn() = default;

            void setItemText(int rowIndex);

        private:
            const CComboBox& comboBox_;
        };



        class CheckBoxColumn : public Column
        {
        public:
            CheckBoxColumn(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex, const CButton& checkBox);
            virtual ~CheckBoxColumn() = default;

            void setItemText(int rowIndex);

        private:
            const CButton& checkBox_;
        };

    }
}

#endif // BKZ_SETTING_COLUMN_IMPL_H
