#include <BeckyAPI.h>
extern CBeckyAPI beckyApi;

#include "UiPresenterImpl.h"

namespace bkzoo
{
    namespace ui
    {

        UiPresenterImpl::UiPresenterImpl(HWND hWnd)
            : hwnd_(hWnd)
        {
        }

        //bool UiPresenterImpl::isEditable() const
        //{
        //	return true;
        //}
    }
}
