/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include "util/RegexSearcher.h"

#include "debug/detect_memory_leak.h"

using namespace bkzoo::util;

namespace tut
{
    // Data used by each test
    struct RegexSearcher_data
    {
        virtual ~RegexSearcher_data()
        {
        }
    };

    // Test group registration
    typedef test_group<RegexSearcher_data> factory;
    typedef factory::object object;
}

namespace
{
    tut::factory tf("RegexSearcher");
}

namespace tut
{

    // RegexSearcher
    template<>
    template<>
    void object::test<1>()
    {
        {
            // wordもregexpstrもどちらも空
            RegexSearcher regexSearcher(L"", L"");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L""));
        }
        {
            // wordだけ空
            RegexSearcher regexSearcher(L"", L"^#?([0-9]+)$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L""));
        }
        {
            // regepだけ空
            RegexSearcher regexSearcher(L"#11", L"");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L"#11"));
        }
        {
            // 正常系
            RegexSearcher regexSearcher(L"#11", L"^#?([0-9]+)$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L"11"));
        }
        {
            // 正常系
            RegexSearcher regexSearcher(L"11", L"^#?([0-9]+)$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L"11"));
        }
        {
            // ヒットなし
            RegexSearcher regexSearcher(L"abcdefg", L"^#?([0-9]+)$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), false);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L""));
        }
        {
            // 正常系 (別のregex)
            RegexSearcher regexSearcher(L"#11", L"^#?(\\d+)$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L"11"));
        }
        {
            // カッコなし
            RegexSearcher regexSearcher(L"#11", L"^#?\\d+$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L"#11"));
        }
        {
            // 正常系 (別のregex)
            RegexSearcher regexSearcher(L"123-4567", L"^(\\d{3}-?\\d{4})$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L"123-4567"));
        }
        {
            // カッコなし
            RegexSearcher regexSearcher(L"123-4567", L"^\\d{3}-?\\d{4}$");
            ensure_equals(M(__LINE__), regexSearcher.validate(), true);
            ensure_equals(M(__LINE__), regexSearcher.getActualSearchWord(), std::wstring(L"123-4567"));
        }
    }

    template<>
    template<>
    void object::test<2>()
    {
        ensure_equals(M(__LINE__), RegexUtil::isValidRegex(L""), true);
        ensure_equals(M(__LINE__), RegexUtil::isValidRegex(L"["), false);
        ensure_equals(M(__LINE__), RegexUtil::isValidRegex(L"ab"), true);
    }

}
