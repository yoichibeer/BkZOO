/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_EXPLORER_H
#define BKZ_COMMAND_EXPLORER_H

#include "Command.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {

        // Explorer呼び出し
        class ExplorerCommand final : public Command
        {
        public:
            explicit ExplorerCommand(const SimpleParam& param);
            virtual bool execute() override;

        private:
            const SimpleParam param_;
        };

    }
}

#endif // BKZ_COMMAND_EXPLORER_H
