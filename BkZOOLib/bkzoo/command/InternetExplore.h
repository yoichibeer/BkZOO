/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_INTERNET_EXPLORE_H
#define BKZ_COMMAND_INTERNET_EXPLORE_H

#include "Command.h"

namespace bkzoo
{
    namespace command
    {

        // IE呼び出し
        class InternetExploreCommand final : public Command
        {
        public:
            explicit InternetExploreCommand(const SimpleParam& param);
            InternetExploreCommand(const SimpleParam& param, unsigned long availableSchemes);
            virtual bool execute() override;

        private:
            const SimpleParam param_;
            const unsigned long m_availableSchemes;
        };

    }
}

#endif // BKZ_COMMAND_INTERNET_EXPLORE_H
