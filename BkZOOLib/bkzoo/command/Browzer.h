/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_BROWZER_H
#define BKZ_COMMAND_BROWZER_H

#include "Command.h"

namespace bkzoo
{
    namespace command
    {

        // Browzer呼び出し
        class BrowzerCommand final : public Command
        {
        public:
            explicit BrowzerCommand(const SimpleParam& param);
            BrowzerCommand(const SimpleParam& param, unsigned long availableSchemes);
            virtual bool execute() override;

        private:
            const SimpleParam param_;
            const unsigned long m_availableSchemes;
        };

    }
}

#endif // BKZ_COMMAND_BROWZER_H
