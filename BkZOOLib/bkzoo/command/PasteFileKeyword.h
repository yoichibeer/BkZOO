/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_PASTE_FILE_KEYWORD_H
#define BKZ_COMMAND_PASTE_FILE_KEYWORD_H

#include "command/Command.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {

        // <file:// > 等を付加して貼り付け
        class PasteFileKeywordCommand final : public Command
        {
        public:
            explicit PasteFileKeywordCommand(const SimpleParam& param);
            virtual bool execute() override;

        private:
            const SimpleParam param_;
        };

    }
}

#endif // BKZ_COMMAND_PASTE_FILE_KEYWORD_H
