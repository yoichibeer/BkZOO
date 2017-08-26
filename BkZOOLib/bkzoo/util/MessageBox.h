/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_MESSAGE_BOX_H
#define BKZ_UTIL_MESSAGE_BOX_H

#include <Windows.h>

namespace bkzoo
{
    namespace util
    {
        /// Yes押下でtrue。それ以外（Noか右上の×でfalse）
        bool okCancelWarningMessageBox(HWND hWnd, UINT uID);

        void messageBox(HWND hWnd, UINT uID);

    }
}

#endif // BKZ_UTIL_MESSAGE_BOX_H
