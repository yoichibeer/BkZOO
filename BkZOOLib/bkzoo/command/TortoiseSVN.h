/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_TORTOISE_SVN_H
#define BKZ_COMMAND_TORTOISE_SVN_H

#include "command/Command.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {

        // TortoiseSNV呼び出し
        class TortoiseSVNCommand final : public Command
        {
        public:
            explicit TortoiseSVNCommand(const SimpleParam& param);
            virtual bool execute() override;

        private:
            const SimpleParam param_;
        };

    }
}

#endif // BKZ_COMMAND_TORTOISE_SVN_H
