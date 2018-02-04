/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "ThreadFunc.h"

#include "command/Command.h"

#include "bkzoo_log.h"

#include <memory>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace thread_func
    {

        void executeCommand(void* arg)
        {
            try
            {
                std::unique_ptr<command::Command> command(static_cast<command::Command*>(arg));
                command->execute();
            }
            catch (std::exception& e)
            {
                LOG_ERROR << e.what();
                assert(false);
            }
            catch (...)
            {
                LOG_ERROR << "command->execute()";
                assert(false);
            }
        }

    }
}
