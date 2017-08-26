#include <gtest/gtest.h>

#include "string/StringUtils.h"

#include <locale>

using namespace bkzoo;

using namespace std::string_literals;

TEST(StringUtils, isPositiveNumber_SUCCESS)
{
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"0"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"1"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"2"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"3"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"4"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"5"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"6"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"7"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"8"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"9"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"12"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"123"s));
    EXPECT_TRUE(string::StringUtils::isPositiveNumber(L"0123"s));

    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-0"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-1"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-2"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-3"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-4"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-5"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-6"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-7"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-8"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-9"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-12"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-123"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"-0123"s));
}

TEST(StringUtils, isPositiveNumber_ERROR)
{
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L""s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L" "s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"a"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"- 1"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"0.1"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"0x11"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"全角文字"s));
}

TEST(StringUtils, isNumber_SUCCESS)
{
    EXPECT_TRUE(string::StringUtils::isNumber(L"0"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"1"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"2"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"3"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"4"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"5"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"6"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"7"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"8"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"9"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"12"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"123"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"0123"s));

    EXPECT_TRUE(string::StringUtils::isNumber(L"-0"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-1"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-2"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-3"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-4"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-5"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-6"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-7"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-8"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-9"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-12"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-123"s));
    EXPECT_TRUE(string::StringUtils::isNumber(L"-0123"s));
}

TEST(StringUtils, isNumber_ERROR)
{
    EXPECT_FALSE(string::StringUtils::isNumber(L""s));
    EXPECT_FALSE(string::StringUtils::isNumber(L" "s));
    EXPECT_FALSE(string::StringUtils::isNumber(L"a"s));
    EXPECT_FALSE(string::StringUtils::isNumber(L"- 1"s));
    EXPECT_FALSE(string::StringUtils::isNumber(L"0.1"s));
    EXPECT_FALSE(string::StringUtils::isNumber(L"0x11"s));
    EXPECT_FALSE(string::StringUtils::isPositiveNumber(L"全角文字"s));
}

TEST(StringUtils, split_to_strings)
{
    // std::wstring
    {
        const std::wstring input = L"abc=123&auto_detect=on&before={}";
        std::vector<std::wstring> tokens = string::StringUtils::split(input, L'&');

        EXPECT_EQ(3U, tokens.size());
        EXPECT_EQ(L"abc=123"s, tokens[0]);
        EXPECT_EQ(L"auto_detect=on"s, tokens[1]);
        EXPECT_EQ(L"before={}"s, tokens[2]);
    }

    // std::string
    {
        const std::string input = "abc=123&auto_detect=on&before={}";
        std::vector<std::string> tokens = string::StringUtils::split(input, '&');

        EXPECT_EQ(3U, tokens.size());
        EXPECT_EQ("abc=123"s, tokens[0]);
        EXPECT_EQ("auto_detect=on"s, tokens[1]);
        EXPECT_EQ("before={}"s, tokens[2]);
    }
}


TEST(StringUtils, split_to_ints)
{
    // 正常系
    {
        const std::vector<int> expected = { 0,5,1 };
        EXPECT_EQ(expected, string::StringUtils::split(L"0,5,1"s));
    }

    // 無効文字含む
    {
        const std::vector<int> expected = { 0,5,1,10 };
        EXPECT_EQ(expected, string::StringUtils::split(L"0,,5,three,1, ,10,あいうえお,あA8"s));
    }
}

//TEST(StringUtils, join)
//{
//    // 正常系
//    {
//        const std::wstring expected = L"0,5,1";
//        EXPECT_EQ(expected, string::StringUtils::join({ 0,5,1 }));
//    }
//
//    // 負数はどうなるんだろう
//    {
//        const std::wstring expected = L"0,5,-1";
//        EXPECT_EQ(expected, string::StringUtils::join({ 0,5,-1 }));
//    }
//}

//#include <boost/algorithm/string/case_conv.hpp>

TEST(StringUtils, to_lower)
{
    //{
    //    std::wstring str(L"ハローabcABC123");
    //    ::boost::algorithm::to_lower(str);
    //    EXPECT_EQ(L"ハローabcabc123", str);
    //}
    {
        std::wstring str(L"ハローabcABC123");
        string::StringUtils::to_lower(str);
        EXPECT_EQ(L"ハローabcabc123", str);
    }

}


TEST(StringUtils, isSpace)
{
    EXPECT_TRUE(std::isspace<wchar_t>(L' ', std::locale()));
    EXPECT_TRUE(std::isspace<wchar_t>(L'\n', std::locale()));
    EXPECT_TRUE(std::isspace<wchar_t>(L'\r', std::locale()));
    EXPECT_TRUE(std::isspace<wchar_t>(L'\t', std::locale()));
    EXPECT_TRUE(std::isspace<wchar_t>(L'　', std::locale()));
    EXPECT_TRUE(std::isspace<wchar_t>(L'\v', std::locale()));
    EXPECT_TRUE(std::isspace<wchar_t>(L'\f', std::locale()));
}


//#include <boost/algorithm/string.hpp>

TEST(StringUtils, trim_left)
{
    {
        std::wstring str(L" \t\r\n\f\v　abcdefg 　");
        EXPECT_EQ(L"abcdefg 　", string::StringUtils::trim_left(str));
    }
    //{
    //    std::wstring str(L" \t\r\n\f\v　abcdefg 　");
    //    boost::algorithm::trim_left(str);
    //    EXPECT_EQ(L"abcdefg 　", str);
    //}
}


TEST(StringUtils, trim_right)
{
    {
        std::wstring str(L"\r\n　 abcdefg \t\r\n\f\v　");
        EXPECT_EQ(L"\r\n　 abcdefg", string::StringUtils::trim_right(str));
    }
    //{
    //    std::wstring str(L"\r\n　 abcdefg \t\r\n\f\v　");
    //    boost::algorithm::trim_right(str);
    //    EXPECT_EQ(L"\r\n　 abcdefg", str);
    //}
}


TEST(StringUtils, trim)
{
    {
        std::wstring str(L" \t\r\n\f\v　a b\tc\rd\ne　f\fg　\v\f\n\t \r   ");
        EXPECT_EQ(L"a b\tc\rd\ne　f\fg", string::StringUtils::trim_left(string::StringUtils::trim_right(str)));
    }
    //{
    //    std::wstring str(L" \t\r\n\f\v　a b\tc\rd\ne　f\fg　\v\f\n\t \r   ");
    //    boost::algorithm::trim(str);
    //    EXPECT_EQ(L"a b\tc\rd\ne　f\fg", str);
    //}
}
