/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_STRINGS_STRING_UTILS_H
#define BKZ_STRINGS_STRING_UTILS_H

#include <string>
#include <vector>
#include <sstream>

#include <tchar.h>

#include "defs.h"

namespace bkzoo
{
    namespace string
    {

#define CW(T, STR) cw<T>(STR, L##STR)

        template<typename T>
        const T* cw(const char* c, const wchar_t* w);

        template<>
        const char* cw(const char* c, const wchar_t* /*w*/);
        template<>
        const wchar_t* cw(const char* /*c*/, const wchar_t* w);

        template<typename T>
        const T cw(const char c, const wchar_t w);

        template<>
        const char cw(const char c, const wchar_t /*w*/);
        template<>
        const wchar_t cw(const char /*c*/, const wchar_t w);


        class StringUtils final
        {
        public:

            // inputをdelimで分割して返却する
            template<typename T>
            static std::vector<T> split(const T& inputStr, typename T::value_type delimiter);

            template<typename T>
            static bool isPositiveNumber(const T& str);

            template<typename T>
            static bool isNumber(const T& str);

            static std::vector<int> split(const std::wstring& str);

            //            static std::wstring join(const std::vector<int>& list);

            static void to_lower(std::wstring& inOut, const std::locale& loc = std::locale());

            static std::wstring& trim_left(std::wstring& str);
            static std::wstring& trim_right(std::wstring& str);
            static std::wstring& trim(std::wstring& str);

        private:
            StringUtils() = default;
            ~StringUtils() = default;

            DISALLOW_COPY_AND_ASSIGN(StringUtils);
        };

        // inputをdelimで分割して返却する
        template<typename T>
        std::vector<T> StringUtils::split(const T& inputStr, typename T::value_type delimiter)
        {
            std::vector<T> tokens;
            T token;
            std::basic_istringstream<T::value_type, T::traits_type, T::allocator_type> stream(inputStr);
            while (std::getline(stream, token, delimiter))
            {
                tokens.push_back(token);
            }
            return tokens;
        }

        template<typename T>
        bool StringUtils::isPositiveNumber(const T& str)
        {
            typedef typename T::value_type char_type;

            if (str.empty())
                return false;

            if (str.find_first_not_of(CW(char_type, "0123456789")) == T::npos)
                return true;

            return false;
        }

        template<typename T>
        bool StringUtils::isNumber(const T& str)
        {
            typedef typename T::value_type char_type;

            if (str.empty())
                return false;

            if (isPositiveNumber(str))
                return true;

            if (str.at(0) != CW(char_type, '-'))
                return false;

            if (str.substr(1, str.size()).find_first_not_of(CW(char_type, "0123456789")) == T::npos)
                return true;

            return false;
        }

    }
}


#endif // BKZ_STRINGS_STRING_UTILS_H
