/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include <tut/tut_console_reporter.hpp>
#include <tut/tut_main.hpp>

#include "string/mlangApi.h"
#include "bkzoo_log.h"

#include <iostream>

#include "for_debug/detect_memory_leak.h"

extern MlangApi mlang;

namespace tut
{
    test_runner_singleton runner;
}

int main(int argc, const char* argv[])
{
#ifdef _DEBUG
    ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif //_DEBUG

    tut::console_reporter reporter;
    tut::runner.get().set_callback(&reporter);

    // ログ初期化
    Logger::createInstanceCallOnceFirst(L"BkZOOTest.tut.log");

    try
    {
        if (tut::tut_main(argc, argv))
        {
            if (reporter.all_ok())
            {
                return 0;
            }
            else
            {
                std::cerr << "\nFAILURE and EXCEPTION" << std::endl;
            }
        }
    }
    catch (const tut::no_such_group &ex)
    {
        std::cerr << "No such group: " << ex.what() << std::endl;
    }
    catch (const tut::no_such_test &ex)
    {
        std::cerr << "No such test: " << ex.what() << std::endl;
    }
    catch (const tut::tut_error &ex)
    {
        std::cout << "General error: " << ex.what() << std::endl;
    }

    return 0;
}
