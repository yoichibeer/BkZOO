/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include "command/helper/SearchWordHelper.h"
#include "util/StringUtil.h"

#include "for_debug/detect_memory_leak.h"

using namespace bkzoo::command::helper;
using namespace bkzoo::util;

namespace tut
{
    // Data used by each test
    struct CommandHelper_data
    {
        CommandHelper_data()
        {
        }
        virtual ~CommandHelper_data()
        {
        }
    };

    // Test group registration
    typedef test_group<CommandHelper_data> factory;
    typedef factory::object object;
}

namespace
{

    tut::factory tf("CommandHelper");

}

namespace tut
{

    // getPostParam
    template<> template<>
    void object::test<2>()
    {
        const std::wstring getUrl = L"http://www.excite.co.jp/world/english/?auto_detect=on&before={}";
        PostParam postParam = getPostParam(getUrl);

        ensure_equals(M(__LINE__), postParam.actionUrl, L"http://www.excite.co.jp/world/english/");
        ensure_equals(M(__LINE__), postParam.params[L"nothing"], L"");
        ensure_equals(M(__LINE__), postParam.params[L"auto_detect"], L"on");
        ensure_equals(M(__LINE__), postParam.params[L"before"], L"{}");
    }

    // stringFromTime
    template<> template<>
    void object::test<3>()
    {
        const std::wstring now = wm_cast<std::wstring>(stringFromTime());

        std::vector<std::wstring> elems = StringUtils::split(now, L'.');
        ensure_equals(M(__LINE__), elems.size(), 6U);
        ensure(M(__LINE__), std::stoi(elems.at(0)) > 2014);
    }

}
