#include "StringUtils.h"

#include <algorithm>
#include <sstream>
#include <locale> // for std::locale, std::tolower

template<>
const char* bkzoo::string::cw(const char* c, const wchar_t* /*w*/)
{
    return c;
}

template<>
const wchar_t* bkzoo::string::cw(const char* /*c*/, const wchar_t* w)
{
    return w;
}

template<>
const char bkzoo::string::cw(const char c, const wchar_t /*w*/)
{
    return c;
}

template<>
const wchar_t bkzoo::string::cw<wchar_t>(const char /*c*/, const wchar_t w)
{
    return w;
}


std::vector<int> bkzoo::string::StringUtils::split(const std::wstring& str)
{
    const wchar_t delimiter(L',');

    std::wistringstream src(str);
    std::wstring dst;
    std::vector<int> list;
    while (std::getline(src, dst, delimiter))
    {
        if (isPositiveNumber(dst))
        {
            list.push_back(std::stoi(dst));
        }
    }
    return list;
}

//std::wstring bkzoo::string::StringUtils::join(const std::vector<int>& list)
//{
//    std::wostringstream stream;
//    bool first = true;
//    for (int i : list)
//    {
//        if (! first)
//        {
//            stream << ",";
//        }
//        else
//        {
//            first = false;
//        }
//        stream << i;
//    }
//    return stream.str();
//}

void bkzoo::string::StringUtils::to_lower(std::wstring& inOut, const std::locale& loc)
{
    for (wchar_t& w : inOut)
    {
        w = std::tolower(w, loc);
    }
}

std::wstring& bkzoo::string::StringUtils::trim_left(std::wstring& str)
{
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), [](wchar_t w) { return std::isspace(w, std::locale()); }));
    return str;
}

std::wstring& bkzoo::string::StringUtils::trim_right(std::wstring& str)
{
    str.erase(std::find_if_not(str.rbegin(), str.rend(), [](wchar_t w) { return std::isspace(w, std::locale()); }).base(), str.end());
    return str;
}

std::wstring& bkzoo::string::StringUtils::trim(std::wstring& str)
{
    return trim_left(trim_right(str));
}

