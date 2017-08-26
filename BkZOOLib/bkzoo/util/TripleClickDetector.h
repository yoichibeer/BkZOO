/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_TRIPLE_CLICK_DETECTOR_H
#define BKZ_UTIL_TRIPLE_CLICK_DETECTOR_H

#include <windows.h>

#include "defs.h"

namespace bkzoo
{
    namespace util
    {

        // TripleClickを判定する。
        class TripleClickDetector final
        {
        public:
            TripleClickDetector();
            ~TripleClickDetector() = default;

            // WM_LBUTTONDBLCLK メッセージ毎にこの関数を呼ぶ
            void start(POINT startPoint);

            // runningは別のスレッドで実行する。interruptされずにDoubleClickTime()経過したらTripleClickと判定する。
            // 返り値がtrueでTripleClick、falseでDoubleClickと判定。
            bool running();

            // WM_LBUTTONDOWN メッセージ毎にinterruptする。
            // 返り値がtrueでTripleClick、falseでSingleClickと判定。
            bool interrupt(POINT endPoint);

        private:
            // Leaveし忘れないために使う。デストラクタで自動的にLeaveする。
            class ScopedCriticalSection final
            {
            public:
                explicit ScopedCriticalSection(CRITICAL_SECTION& criticalSection);
                ~ScopedCriticalSection();

            private:
                CRITICAL_SECTION& criticalSection_;

                DISALLOW_COPY_AND_ASSIGN(ScopedCriticalSection);
            };

        private:
            POINT doubleClickStartPoint_;
            DWORD doubleClickStartTickCount_; // millisecond
            POINT endPoint_;
            bool isInterrupted_;
            bool isTripleClicked_;

            CRITICAL_SECTION criticalSection_;

        private:
            // TripleClickの3回目のクリックが2回目から一定時間以内であればtrueを返却
            bool withinDoubleClickTimeFromDoubleClickStartedTime() const;

            // トリプルクリック中にマウス位置の変動が閾値以内であればtrueを返却する
            bool withinTripleClickPoint() const;

            DISALLOW_COPY_AND_ASSIGN(TripleClickDetector);
        };

    }
}

#endif // BKZ_UTIL_TRIPLE_CLICK_DETECTOR_H
