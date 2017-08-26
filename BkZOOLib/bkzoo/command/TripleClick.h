/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_TRIPLE_CLICK_H
#define BKZ_COMMAND_TRIPLE_CLICK_H

#include "command/Command.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {
        typedef struct TripleClickParam
        {
            SimpleParam simpleParam;
            unsigned long modifierKey;
        } TripleClickParam;

        // TripleClick時の処理
        class TripleClickCommand final : public Command
        {
        public:
            explicit TripleClickCommand(const TripleClickParam& param);
            virtual bool execute() override;

        private:
            TripleClickParam param_;
        };

    }
}

#endif // BKZ_COMMAND_TRIPLE_CLICK_H
