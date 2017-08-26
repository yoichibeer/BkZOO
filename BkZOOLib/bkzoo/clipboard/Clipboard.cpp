/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Clipboard.h"

#include "bkzoo_log.h"

#include <stdexcept>

#include <cassert>
#include "debug/detect_memory_leak.h"


namespace bkzoo
{
    namespace clipboard
    {
        enum : int
        {
            LOOP_NUM = 5,
            SLEEP_TIME_ms = 10,
        };

        ScopedClipboard::ScopedClipboard(HWND hWnd)
            : isClosed_(FALSE)
        {
            for (int i = 0; i < LOOP_NUM; ++i)
            {
                if (::OpenClipboard(hWnd) != FALSE)
                {
                    return;
                }
                ::Sleep(SLEEP_TIME_ms);
            }

            LOG_ERROR << "OpenClipboard(hWnd), hWnd=" << hWnd;
            throw std::runtime_error("OpenClipboard ERROR!");
        }


        ScopedClipboard::~ScopedClipboard()
        {
            close();
        }

        std::wstring ScopedClipboard::getClipboardData() const
        {
            if (isClosed_ == TRUE)
            {
                return std::wstring();
            }

            HANDLE hText = ::GetClipboardData(CF_UNICODETEXT);
            if (hText == nullptr)
            {
                return std::wstring();
            }

            const std::wstring str(static_cast<wchar_t*>(::GlobalLock(hText)));
            ::GlobalUnlock(hText);
            return str;
        }

        BOOL ScopedClipboard::emptyClipboard() const
        {
            if (isClosed_ == TRUE)
            {
                return FALSE;
            }

            return ::EmptyClipboard();
        }

        HANDLE ScopedClipboard::setClipboardData(const std::wstring& str)
        {
            if (isClosed_ == TRUE)
            {
                return nullptr;
            }

            // クリップボードに渡すメモリ準備
            const size_t dstLen = str.length() + 1; // 1はnullptr終端文字分
            HGLOBAL hText = ::GlobalAlloc(GMEM_MOVEABLE, dstLen * sizeof(wchar_t));
            if (hText == nullptr)
            {
                return nullptr;
            }

            wchar_t* pText = static_cast<wchar_t*>(::GlobalLock(hText));
            wcsncpy_s(pText, dstLen, str.c_str(), _TRUNCATE);
            ::GlobalUnlock(hText);

            // クリップボードにセット
            HANDLE handle = ::SetClipboardData(CF_UNICODETEXT, hText);
            if (handle == nullptr)
            {
                ::GlobalFree(hText);
            }

            return handle;
        }

        BOOL ScopedClipboard::close()
        {
            if (isClosed_ == TRUE)
            {
                return FALSE;
            }

            isClosed_ = ::CloseClipboard();
            return isClosed_;
        }


        RevertClipboard::RevertClipboard(HWND hWnd)
            : reverted_(FALSE), hWnd_(hWnd)
        {
            ScopedClipboard scopedClipboard(hWnd_);

            // RevertClipboardスコープを抜ける際に復帰する値を取得する
            clipboardData_ = scopedClipboard.getClipboardData();
            scopedClipboard.emptyClipboard();

            if (scopedClipboard.close() == FALSE)
            {
                return;
            }
        }

        RevertClipboard::~RevertClipboard()
        {
            if (reverted_ == TRUE)
            {
                return;
            }

            try
            {
                revert();
            }
            catch (...)
            {
                LOG_ERROR << "revert()";
                assert(false);
            }
        }

        void RevertClipboard::revert()
        {
            ScopedClipboard scopedClipboard(hWnd_);

            // RevertClipboard呼び出し前のクリップボードに値があってもなくても共通する処理として空にする
            scopedClipboard.emptyClipboard();

            // RevertClipboard呼び出し前が空の場合
            if (clipboardData_.empty())
            {
                return;
            }

            // RevertClipboard呼び出し前に値があった場合は復帰する
            scopedClipboard.setClipboardData(clipboardData_);
            reverted_ = TRUE;
        }

    }
}
