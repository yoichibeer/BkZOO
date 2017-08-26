#ifndef BKZ_UI_PRESENTER_H
#define BKZ_UI_PRESENTER_H

#include "defs.h"

namespace bkzoo
{
    namespace ui
    {

        /**
         * 2017/05/20
         * クリーンアーキテクチャの中心から外へのAdapterであるPresenterをつくってみようとしてみた。
         * HWNDが内部に入り込んでいるので外に出そうというのが狙い。HWNDのラッパーになる見込み。
         */
        class UiPresenter
        {
        public:
            UiPresenter() = default;
            virtual ~UiPresenter() = default;

            virtual bool isEditable() const = 0;

        private:
            DISALLOW_COPY_AND_ASSIGN(UiPresenter);
        };

    }
}

#endif // BKZ_UI_PRESENTER_H
