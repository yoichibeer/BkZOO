/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_URL_URL_ENCODE_H
#define BKZ_URL_URL_ENCODE_H

#include <Windows.h>

#include <string>

namespace bkzoo
{
    namespace url
    {

        typedef std::string(*fn_urlEncode)(const std::string&);

        std::string urlencode(const std::string& str);

    }
}

#endif // BKZ_URL_URL_ENCODE_H
