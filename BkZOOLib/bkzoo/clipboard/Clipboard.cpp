/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Clipboard.h"

#include "bkzoo_string.h"
#include "bkzoo_log.h"

#include <stdexcept>

#include <iostream>

#include "for_debug/detect_memory_leak.h"


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
                // OpenClipboard()に失敗することがあるので繰り返す。
                if (::OpenClipboard(hWnd) == TRUE)
                    return;

                LOG_INFO << "i=[" << i << "]";
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
                LOG_INFO << "isClosed_ is true.";
                return std::wstring();
            }

            HANDLE hClipBoardText = ::GetClipboardData(CF_UNICODETEXT);
            if (hClipBoardText == nullptr)
            {
                /// @note (yoichi) 2018/01/21 文字列選択してSendMessage送っているのにGetClipboardData()が、
                /// nullptrを返すことがあって、その場合は右クリックでメニューに追加されない。
                /// ただ、以下で::GetLastError()は0を返却するため原因が不明。
                /// SendMessage() の前でSleepするとなぜか防げるためとりあえず実施。
                LPVOID lpMessageBuffer = nullptr;
                const DWORD lastError = ::GetLastError();
                LOG_ERROR << "GetClipboardData() failed. GetLastError:" << lastError;
                ::FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    lastError,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト ユーザー言語
                    reinterpret_cast<LPWSTR>(&lpMessageBuffer),
                    0,
                    nullptr
                );
                LOG_ERROR << wm_cast<std::string>(static_cast<LPCWSTR>(lpMessageBuffer));
                // システムによって確保されたバッファを開放します。
                ::LocalFree(lpMessageBuffer);

                return std::wstring();
            }

            const std::wstring clipboardData(static_cast<wchar_t*>(::GlobalLock(hClipBoardText)));
            ::GlobalUnlock(hClipBoardText);
            LOG_INFO << "clipboardData=" << clipboardData;
            return clipboardData;
        }

        BOOL ScopedClipboard::emptyClipboard() const
        {
            if (isClosed_ == TRUE)
            {
                LOG_INFO << "isClosed_ is true.";
                return FALSE;
            }

            return ::EmptyClipboard();
        }

        HANDLE ScopedClipboard::setClipboardData(const std::wstring& str)
        {
            if (isClosed_ == TRUE)
            {
                LOG_INFO << "isClosed_ is true.";
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
            const HANDLE handle = ::SetClipboardData(CF_UNICODETEXT, hText);
            if (handle == nullptr)
            {
                //　SetClipboardData()に成功したらリソース管理はシステムにまかせるが、失敗したら自前でFreeする。
                ::GlobalFree(hText);
            }

            return handle;
        }

        BOOL ScopedClipboard::close()
        {
            if (isClosed_ == TRUE)
            {
                LOG_INFO << "isClosed_ is true.";
                return FALSE;
            }

            isClosed_ = ::CloseClipboard();
            if (isClosed_ == TRUE)
                return TRUE;

            LOG_ERROR << "close() failed";
            isClosed_ = TRUE;
            return TRUE;
        }


        RevertClipboard::RevertClipboard(HWND hWnd)
            : reverted_(FALSE), hWnd_(hWnd)
        {
            ScopedClipboard scopedClipboard(hWnd_);

            // RevertClipboardスコープを抜ける際に復帰する値を取得する
            clipboardData_ = scopedClipboard.getClipboardData();
            if (scopedClipboard.emptyClipboard() == FALSE)
                LOG_ERROR << "emptyClipboard() failed.";
        }

        RevertClipboard::~RevertClipboard()
        {
            if (reverted_ == TRUE)
            {
                LOG_INFO << "reverted_ is true.";
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
            if (reverted_ == TRUE)
                return;

            reverted_ = TRUE;

            ScopedClipboard scopedClipboard(hWnd_);

            // RevertClipboard呼び出し前のクリップボードに値があってもなくても共通する処理として空にする
            if (scopedClipboard.emptyClipboard() == FALSE)
                LOG_ERROR << "emptyClipboard() failed.";

            // RevertClipboard呼び出し前が空の場合
            if (clipboardData_.empty())
                return;

            // RevertClipboard呼び出し前に値があった場合は復帰する
            scopedClipboard.setClipboardData(clipboardData_);
        }

    }
}
