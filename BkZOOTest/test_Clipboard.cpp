/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include "clipboard/Clipboard.h"

#include "for_debug/detect_memory_leak.h"

using namespace bkzoo::clipboard;

namespace tut
{

    // Data used by each test
    struct Clipboard_data
    {
        Clipboard_data()
        {
            ScopedClipboard scopedClipboard(nullptr);
            scopedClipboard.emptyClipboard();
        }
        virtual ~Clipboard_data()
        {
            ScopedClipboard scopedClipboard(nullptr);
            scopedClipboard.emptyClipboard();
        }
    };

    // Test group registration
    typedef test_group<Clipboard_data> factory;
    typedef factory::object object;

}

namespace
{

    tut::factory tf("Clibboard");

}

namespace tut
{

    // ScopedClipboard
    template<>
    template<>
    void object::test<1>()
    {
        try
        {
            // クリップボードに値設定
            std::wstring expected(L"ClipboardWrapperTest");
            {
                ScopedClipboard scopedClipboard(nullptr);
                scopedClipboard.setClipboardData(expected);
            }

            // クリップボードから値取得
            std::wstring actual;
            {
                ScopedClipboard scopedClipboard(nullptr);
                actual = scopedClipboard.getClipboardData();
            }
            ensure_equals(M(__LINE__), actual, expected);
        }
        catch (std::exception& e)
        {
            fail(e.what());
        }
        catch (...)
        {
            fail(M(__LINE__) + " exception");
        }
    }

    // RevertClipboard
    template<>
    template<>
    void object::test<2>()
    {
        try
        {
            // RevertClipboard呼び出し前のクリップボードが空の場合の処理
            RevertClipboard revertClipboard(nullptr);

            // RevertClipboardスコープ内でクリップボードに値設定
            std::wstring dummy(L"SetClipboardDataWithinRevertClipboard");
            ScopedClipboard scopedClipboard(nullptr);
            scopedClipboard.setClipboardData(dummy);
            revertClipboard.revert();
            revertClipboard.revert();
            revertClipboard.revert();

            // RevertClipboardスコープを抜けてクリップボードが空か確認
            std::wstring actual;
            {
                ScopedClipboard scopedClipboard2(nullptr);
                actual = scopedClipboard2.getClipboardData();
            }
            ensure_equals(M(__LINE__), actual, std::wstring());
        }
        catch (std::exception& /* e */)
        {
            throw;
        }
        catch (...)
        {
            fail(M(__LINE__) + " exception  fail");
        }
    }

    // RevertClipboard
    template<>
    template<>
    void object::test<3>()
    {
        try
        {
            // クリップボードに値設定
            std::wstring expected(L"RevertClipboardTest");
            {
                ScopedClipboard scopedClipboard(nullptr);
                scopedClipboard.setClipboardData(expected);
            }

            // RevertClipboardスコープ内でクリップボードに別の値設定
            {
                RevertClipboard revertClipboard(nullptr);

                // RevertClipboardスコープ内でクリップボードに別の値設定
                std::wstring expectedInScope(L"SetClipboardDataWithinRevertClipboard");
                ScopedClipboard scopedClipboard(nullptr);
                scopedClipboard.setClipboardData(expectedInScope);
                scopedClipboard.close();
                scopedClipboard.close();
                scopedClipboard.close();

                // RevertClipboardスコープ内でクリップボードから値取得
                std::wstring actualInScope;
                {
                    ScopedClipboard scopedClipboard2(nullptr);
                    actualInScope = scopedClipboard2.getClipboardData();
                }
                ensure_equals(M(__LINE__), actualInScope, expectedInScope);
            }

            // RevertClipboardスコープを抜けてクリップボードから値取得
            std::wstring actual;
            {
                ScopedClipboard scopedClipboard(nullptr);
                actual = scopedClipboard.getClipboardData();
            }
            ensure_equals(M(__LINE__), actual, expected);
        }
        catch (std::exception& /* e */)
        {
            throw;
        }
        catch (...)
        {
            fail(M(__LINE__) + " exception");
        }
    }

}
