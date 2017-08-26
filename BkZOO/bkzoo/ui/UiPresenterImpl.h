#ifndef BKZ_UI_PRESENTER_IMPL_H
#define BKZ_UI_PRESENTER_IMPL_H

#include "bkzoo/ui/UiPresenter.h"

#include <Windows.h>

namespace bkzoo
{
    namespace ui
    {

        class UiPresenterImpl : public UiPresenter
        {
        public:
            explicit UiPresenterImpl(HWND hWnd);
            virtual ~UiPresenterImpl() = default;

            //			virtual bool isEditable() const override;

        private:
            HWND hwnd_;
        };

    }
}

#endif // BKZ_UI_PRESENTER_IMPL_H
