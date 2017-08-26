/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include "url/URLEncode.h"

#include "debug/detect_memory_leak.h"

namespace tut
{

    // Data used by each test
    struct URLEncode_data
    {
        virtual ~URLEncode_data()
        {
        }
    };

    // Test group registration
    typedef test_group<URLEncode_data> factory;
    typedef factory::object object;

}

namespace
{

    tut::factory tf("URLEncode");

}

namespace tut
{

    // encodedUrl
    template<>
    template<>
    void object::test<1>()
    {
        std::string encodedUrl = ("azAZ09-_.!~*'()");
        ensure_equals(M(__LINE__), encodedUrl, "azAZ09-_.!~*'()");
    }

}
