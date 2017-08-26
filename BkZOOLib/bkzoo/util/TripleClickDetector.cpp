/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "TripleClickDetector.h"

#include "bkzoo_log.h"

#include <process.h>

namespace bkzoo
{
    namespace util
    {

        TripleClickDetector::TripleClickDetector()
            : doubleClickStartPoint_({ 0, 0 }), doubleClickStartTickCount_(0)
            , endPoint_({ 0, 0 }), isInterrupted_(false), isTripleClicked_(false)
        {
            ::InitializeCriticalSection(&criticalSection_);
        }

        void TripleClickDetector::start(POINT startPoint)
        {
            // 全てのメンバ変数を初期化する
            doubleClickStartPoint_ = startPoint;
            doubleClickStartTickCount_ = ::GetTickCount();
            endPoint_.x = 0;
            endPoint_.y = 0;
            {
                ScopedCriticalSection scopedCS(criticalSection_);
                isInterrupted_ = false;
            }
            isTripleClicked_ = false;
        }

        bool TripleClickDetector::running()
        {
            while (withinDoubleClickTimeFromDoubleClickStartedTime())
            {
                {
                    ScopedCriticalSection scopedCS(criticalSection_);
                    if (isInterrupted_) // DoubleClickTime期間内にinterruptされた
                    {
                        // TripleClickと判定
                        isTripleClicked_ = withinTripleClickPoint();
                        break;
                    }
                }

                Sleep(10);
                continue;
            }

            {
                ScopedCriticalSection scopedCS(criticalSection_);
                if (isInterrupted_) // 最後のwithinDoubleClickTimeFromDoubleClickStartedTime()の時にinterruptされる場合がある
                {
                    isTripleClicked_ = withinTripleClickPoint();
                }
                else
                {
                    isInterrupted_ = true;
                    isTripleClicked_ = false;
                }
                return isTripleClicked_;
            }
        }

        bool TripleClickDetector::withinDoubleClickTimeFromDoubleClickStartedTime() const
        {
            const DWORD elapsedTime_ms = ::GetTickCount() - doubleClickStartTickCount_;
            if (elapsedTime_ms <= ::GetDoubleClickTime())
            {
                // 3回目のクリックが2回目からGetDoubleClickTime()時間以内ならtrueを返却
                return true;
            }
            return false;
        }

        bool TripleClickDetector::withinTripleClickPoint() const
        {
            // 以下の2という値はDanaエディタは固定値っぽい。この値に::GetSystemMetrics(SM_CXDOUBLECLK);は使ってなさそう。
            int diffX = doubleClickStartPoint_.x - endPoint_.x;
            if ((diffX < -2) || (2 < diffX))
            {
                return false;
            }
            int diffY = doubleClickStartPoint_.y - endPoint_.y;
            if ((diffY < -2) || (2 < diffY))
            {
                return false;
            }

            return true;
        }

        bool TripleClickDetector::interrupt(POINT endPoint)
        {
            endPoint_ = endPoint;

            {
                ScopedCriticalSection scopedCS(criticalSection_);
                if (isInterrupted_) // すでにrunning()は終了し検出は完了しているという意味
                {
                    isTripleClicked_ = false;
                }
                else
                {
                    isInterrupted_ = true; // isInterrupted_への値の設定自体がinterrupt
                    isTripleClicked_ = withinTripleClickPoint();
                }
            }

            return isTripleClicked_;
        }

        TripleClickDetector::ScopedCriticalSection::ScopedCriticalSection(CRITICAL_SECTION& criticalSection)
            : criticalSection_(criticalSection)
        {
            ::EnterCriticalSection(&criticalSection_);
        }
        TripleClickDetector::ScopedCriticalSection::~ScopedCriticalSection()
        {
            ::LeaveCriticalSection(&criticalSection_);
        }

    }
}
