/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_CALLBACK_CALLBACK_H
#define BKZ_CALLBACK_CALLBACK_H

#include <Windows.h>

namespace bkzoo
{
    namespace callback
    {

        void WINAPI ffftp(HWND hWnd, LPARAM lParam);

        void WINAPI fileZilla(HWND hWnd, LPARAM lParam);

        void WINAPI winSCP(HWND hWnd, LPARAM lParam);

        void WINAPI teraTerm(HWND hWnd, LPARAM lParam);

        void WINAPI pasteFileKeyword(HWND hWnd, LPARAM lParam);

        void WINAPI googleSearch(HWND hWnd, LPARAM lParam);

        void WINAPI svn(HWND hWnd, LPARAM lParam);

        void WINAPI remoteDesktop(HWND hWnd, LPARAM lParam);

        void WINAPI browzer(HWND hWnd, LPARAM lParam);

        void WINAPI internetExplore(HWND hWnd, LPARAM lParam);

        void WINAPI explorer(HWND hWnd, LPARAM lParam);

        void WINAPI site(HWND hWnd, LPARAM lParam);

    }
}

#endif //BKZ_CALLBACK_CALLBACK_H
