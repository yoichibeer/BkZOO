/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_REGEX_SEARCHER_H
#define BKZ_UTIL_REGEX_SEARCHER_H

#include "defs.h"

#include <string>

namespace bkzoo
{
    namespace util
    {

        class RegexUtil final
        {
        public:
            static bool isValidRegex(const std::wstring& regexString);

        private:
            DISALLOW_COPY_AND_ASSIGN(RegexUtil);
        };

        class RegexSearcher final
        {
        public:
            RegexSearcher(const std::wstring& word, const std::wstring& regex);
            ~RegexSearcher() = default;

            bool validate() const;

            std::wstring getActualSearchWord() const;

        private:
            bool isValid_;
            std::wstring actualSearchWord_; //実際に検索で用いる文字列、例えば "#11" => "11"

        private:
            DISALLOW_COPY_AND_ASSIGN(RegexSearcher);
        };

    }
}

#endif // BKZ_UTIL_REGEX_SEARCHER_H
