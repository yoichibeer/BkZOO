/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_COMMAND_H
#define BKZ_COMMAND_COMMAND_H

#include <Windows.h>

#include <string>

#include "defs.h"

namespace bkzoo
{
    namespace command
    {

        class Command
        {
        public:
            Command() = default;
            virtual ~Command() = default;
            virtual bool execute() = 0;
        private:
            DISALLOW_COPY_AND_ASSIGN(Command);
        };

        typedef struct SimpleParam
        {
            HWND hWnd;
            std::wstring string;
        } SimpleParam;

    }
}

#endif // BKZ_COMMAND_COMMAND_H
