#include "BackgroundColor.h"

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace setting
    {

        BackgroundColor::BackgroundColor()
        {
            // FormViewの背景は白で決め打ち
            hbrush_ = ::CreateSolidBrush(RGB(255, 255, 255));
        }


        BackgroundColor::~BackgroundColor()
        {
            ::DeleteObject(hbrush_);
        }

        HBRUSH BackgroundColor::hbrush() const
        {
            return hbrush_;
        }

    } // namespace setting
} // namespace bkzoo
