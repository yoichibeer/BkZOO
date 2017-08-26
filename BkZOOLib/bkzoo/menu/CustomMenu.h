/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_MENU_CUSTOM_MENU_H
#define BKZ_MENU_CUSTOM_MENU_H

#include <Windows.h>

namespace bkzoo
{
    namespace menu
    {

        BOOL createCustomMenu(HWND hWnd, HMENU appendedMenu, bool isEditableView, int nType);

    }
}

#endif // BKZ_MENU_CUSTOM_MENU_H
