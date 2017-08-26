/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_CLIPBOARD_CLIPBOARD_H
#define BKZ_CLIPBOARD_CLIPBOARD_H

#include <Windows.h>

#include <string>

#include "defs.h"

namespace bkzoo
{
    namespace clipboard
    {

        // ::OpenClipboard()と::CloseClipboard()をそれぞれコンストラクタとデストラクタで実施するだけ
        class ScopedClipboard final
        {
        public:
            explicit ScopedClipboard(HWND hWnd);
            ~ScopedClipboard(); // non virtual

            std::wstring getClipboardData() const;
            BOOL emptyClipboard() const;
            HANDLE setClipboardData(const std::wstring& str);
            BOOL close();

        private:
            BOOL isClosed_;

        private:
            DISALLOW_COPY_AND_ASSIGN(ScopedClipboard);
        };

        // クリップボードの中身をRevertClipboardオブジェクト生成前の状態に復帰する
        // RevertClipboardオブジェクト生存スコープ中の操作はオブジェクト破棄後に
        class RevertClipboard final
        {
        public:
            explicit RevertClipboard(HWND hWnd);
            ~RevertClipboard(); // non virtual

            void revert();

        private:
            BOOL reverted_;
            HWND hWnd_;
            std::wstring clipboardData_;

        private:
            DISALLOW_COPY_AND_ASSIGN(RevertClipboard);
        };

    }
}

#endif //BKZ_CLIPBOARD_CLIPBOARD_H
