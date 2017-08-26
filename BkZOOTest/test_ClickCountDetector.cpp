/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include "util/TripleClickDetector.h"

#include <thread>
#include <future>

#include "debug/detect_memory_leak.h"

using namespace bkzoo::util;

namespace tut
{
    // Data used by each test
    struct TripleClickDetector_data
    {
        virtual ~TripleClickDetector_data()
        {
        }
    };

    // Test group registration
    typedef test_group<TripleClickDetector_data> factory;
    typedef factory::object object;
}

namespace
{
    tut::factory tf("TripleClickDetector");
}

namespace tut
{

    bool running(TripleClickDetector& tripleClickDetector)
    {
        return tripleClickDetector.running();
    }

    template<>
    template<>
    void object::test<1>()
    {
        set_test_name("start()もrunning()もしてない時にWM_LBUTTONDOWNでinterrupt()してfalseが返る");

        TripleClickDetector tripleClickDetector;

        POINT startPoint;
        startPoint.x = 10;
        startPoint.y = 10;

        POINT endPoint(startPoint);

        // WM_LBUTTONDOWNが来たら
        ensure_equals(M(__LINE__), tripleClickDetector.interrupt(endPoint), false);
    }

    template<>
    template<>
    void object::test<2>()
    {
        set_test_name("start()したけどrunning()してない時にWM_LBUTTONDOWNでinterrupt()してtrueが返る");

        TripleClickDetector tripleClickDetector;

        POINT startPoint;
        startPoint.x = 10;
        startPoint.y = 10;

        POINT endPoint(startPoint);

        // WM_LBUTTONDBLCLKが来たら
        tripleClickDetector.start(startPoint);

        // runningしないで

        // WM_LBUTTONDOWNが来たら
        ensure_equals(M(__LINE__), tripleClickDetector.interrupt(endPoint), true);
    }


    template<>
    template<>
    void object::test<3>()
    {
        set_test_name("start()後にWM_LBUTTONDBLCLKでrunning()してinterruct()されないでfalseが返る");

        TripleClickDetector tripleClickDetector;

        POINT startPoint;
        startPoint.x = 10;
        startPoint.y = 10;

        // WM_LBUTTONDBLCLKが来たら
        tripleClickDetector.start(startPoint);

        // すぐ別スレッドで
        auto result = std::async(std::launch::async, running, std::ref(tripleClickDetector));

        // ここでinterruptされない

        // running()終了
        bool isTripleClicked = result.get();

        ensure_equals(M(__LINE__), isTripleClicked, false);
    }

    template<>
    template<>
    void object::test<4>()
    {
        set_test_name("WM_LBUTTONDBLCLKでrunning()してすぐにWM_LBUTTONDOWNでinterruct()されてtrueが返る");

        TripleClickDetector tripleClickDetector;

        POINT startPoint;
        startPoint.x = 10;
        startPoint.y = 10;

        POINT endPoint(startPoint);


        // WM_LBUTTONDBLCLKが来たら
        tripleClickDetector.start(startPoint);

        // すぐ別スレッドで
        auto result = std::async(std::launch::async, running, std::ref(tripleClickDetector));

        // running()の最中に
        bool isTripleClicked = tripleClickDetector.interrupt(endPoint);
        ensure_equals(M(__LINE__), isTripleClicked, true);

        // running()終了
        isTripleClicked = result.get();
        ensure_equals(M(__LINE__), isTripleClicked, true);
    }

    template<>
    template<>
    void object::test<5>()
    {
        set_test_name("WM_LBUTTONDBLCLKでrunning()してトリプルクリックにならない間隔以上あけるのでWM_LBUTTONDOWNでinterruct()されるがfalseが返る");

        TripleClickDetector tripleClickDetector;

        POINT startPoint;
        startPoint.x = 10;
        startPoint.y = 10;

        POINT endPoint(startPoint);

        // WM_LBUTTONDBLCLKが来たら
        tripleClickDetector.start(startPoint);

        // すぐ別スレッドで
        auto result = std::async(std::launch::async, running, std::ref(tripleClickDetector));

        ::Sleep(600); // DoubleClickTime() は 500ms を返す模様なので、それより大きな値を設定。

        // running()が終わってから
        bool isTripleClicked = tripleClickDetector.interrupt(endPoint);
        ensure_equals(M(__LINE__), isTripleClicked, false);
        ensure_equals(M(__LINE__), result.get(), false);
    }


    template<>
    template<>
    void object::test<6>()
    {
        set_test_name("WM_LBUTTONDBLCLKでrunning()してすぐにWM_LBUTTONDOWNでinterruct()されるがトリプルクリックのマウス位置がずれるのでfalseが返る");

        TripleClickDetector tripleClickDetector;

        POINT startPoint;
        startPoint.x = 10;
        startPoint.y = 10;

        POINT endPoint;
        endPoint.x = 13; // 閾値の2以上にずれている。
        endPoint.y = 10;


        // WM_LBUTTONDBLCLKが来たら
        tripleClickDetector.start(startPoint);

        // すぐ別スレッドで
        auto result = std::async(std::launch::async, running, std::ref(tripleClickDetector));

        // running()の最中に
        bool isTripleClicked = tripleClickDetector.interrupt(endPoint);
        ensure_equals(M(__LINE__), isTripleClicked, false);

        // running()終了
        isTripleClicked = result.get();
        ensure_equals(M(__LINE__), isTripleClicked, false);
    }

}
