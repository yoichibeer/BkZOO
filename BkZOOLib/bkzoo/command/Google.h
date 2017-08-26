/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_GOOGLE_H
#define BKZ_COMMAND_GOOGLE_H

#include "command/Command.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {

        // Googleで検索
        class GoogleCommand final : public Command
        {
        public:
            explicit GoogleCommand(const SimpleParam& param);
            virtual bool execute() override;

        private:
            const SimpleParam param_;
        };

    }
}

#endif // BKZ_COMMAND_GOOGLE_H
