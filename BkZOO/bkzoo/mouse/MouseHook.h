/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_MOUSE_MOUSE_HOOK_H
#define BKZ_MOUSE_MOUSE_HOOK_H

#include <Windows.h>

namespace bkzoo
{
    namespace mouse
    {

        LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam);

    }
}

#endif // BKZ_MOUSE_MOUSE_HOOK_H
