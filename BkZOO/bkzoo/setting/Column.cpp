/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "ColumnImpl.h"

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace setting
    {

        Column::Column(CListViewCtrl& listViewCtrl, enum ColumnIndex columnIndex)
            : listViewCtrl_(listViewCtrl), columnIndex_(columnIndex)
        {
        }

    }
}
