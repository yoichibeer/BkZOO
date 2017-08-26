/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_REMOTE_DESKTOP_H
#define BKZ_COMMAND_REMOTE_DESKTOP_H

#include "Command.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {

        // RemoteDesktop呼び出し
        class RemoteDesktopCommand final : public Command
        {
        public:
            explicit RemoteDesktopCommand(const SimpleParam& param);
            virtual bool execute() override;

        private:
            const SimpleParam param_;
        };

    }
}

#endif // BKZ_COMMAND_REMOTE_DESKTOP_H
