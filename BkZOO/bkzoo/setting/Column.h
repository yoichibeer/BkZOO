/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_SETTING_COLUMN_H
#define BKZ_SETTING_COLUMN_H

#include "wtlheader.h"

namespace bkzoo
{
    namespace setting
    {

        enum ColumnIndex : int
        {
            OnOff,
            Preset,
            Title,
            Shortcut,
            URL,
            Encode,
            Method,
            Regex,
            Clickable,
        };

        // コントロールの値を取得して各Columnに反映する。
        class Column
        {
        public:
            Column(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex);
            virtual ~Column() = default;

            virtual void setItemText(int rowIndex) = 0;

        protected:
            CListViewCtrl& listViewCtrl_;
            const enum ColumnIndex columnIndex_;

        private:
            Column& operator=(const Column&) = delete;
            Column(const Column&) = delete;
        };

    }
}

#endif // BKZ_SETTING_COLUMN_H
