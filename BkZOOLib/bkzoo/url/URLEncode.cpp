/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "URLEncode.h"

#include <sstream>

#include <cassert>
#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace url
    {

        // http://d.hatena.ne.jp/ytakano/20081016
        // http://www.y-adagio.com/public/standards/tr_uri_2396/rfc2396-main.htm#2
        std::string urlencode(const std::string &str)
        {
            std::ostringstream os;

            for (size_t i = 0; i < str.size(); i++)
            {
                char c = str[i];
                if ((c >= 'A' && c <= 'Z') ||
                    (c >= 'a' && c <= 'z') ||
                    (c >= '0' && c <= '9') ||
                    c == '-' ||
                    c == '_' ||
                    c == '.' ||
                    c == '!' ||
                    c == '~' ||
                    c == '*' ||
                    c == '\'' ||
                    c == '(' ||
                    c == ')')
                {
                    os << c;
                }
                else
                {
                    char s[4] = { '\0' };
#ifndef NDEBUG
                    int ret =
#endif //NDEBUG
                        ::sprintf_s(s, sizeof(s), "%%%02x", c & 0xff);
                    assert(3 == ret);
                    os << s;
                }
            }

            return os.str();
        }

    }
}
