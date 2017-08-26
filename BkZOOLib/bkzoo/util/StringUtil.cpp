/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "StringUtil.h"

#include <sstream>

#include <ctime>

using namespace bkzoo;

namespace bkzoo
{
    namespace util
    {

        std::string stringFromTime()
        {
            const time_t now = time(nullptr);
            const struct tm *pnow = localtime(&now);

            const int year = pnow->tm_year + 1900;  /* 1900 - */
            const int month = pnow->tm_mon + 1;  /* 0-11 */
            const int day = pnow->tm_mday + 1;  /* 0-30 */
            const int hour = pnow->tm_hour;  /* 0-23 */
            const int min = pnow->tm_min;  /* 0-59 */
            const int sec = pnow->tm_sec;  /* 0-59 */

            std::ostringstream oss;
            oss << year << "." << month << "." << day << "." << hour << "." << min << "." << sec;
            return oss.str();
        }

    }
}
