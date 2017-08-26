/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_DOUBLE_CLICK_H
#define BKZ_COMMAND_DOUBLE_CLICK_H

#include "Command.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {

        typedef struct DoubleClickParam
        {
            HWND hWnd;
            unsigned long modifierKey;
        } DoubleClickParam;


        // WM_LBUTTONDBLCLKメッセージ受信時に実行されるクラス.
        class DoubleClickCommand final : public Command
        {
        public:
            explicit DoubleClickCommand(const DoubleClickParam& param);
            virtual bool execute() override;

        private:
            const DoubleClickParam param_;
        };

    }
}

#endif // BKZ_COMMAND_DOUBLE_CLICK_H
