#include <gtest/gtest.h>

#include "util/PathUtils.h"

#include <filesystem>

using namespace bkzoo::util;

TEST(PathUtils, modulePath)
{
    fs::path modulePath = PathUtils::modulePath(nullptr);
    EXPECT_EQ(L"UnitTest.exe", modulePath.filename().wstring());
    EXPECT_LT(12U/*UnitTest.exeの文字数*/, modulePath.string().size());
}

TEST(PathUtils, extensionReplacedPath)
{
    {
        // 正常系
        fs::path replacedPath = PathUtils::extensionReplacedPath(L"C:\\ABC\\def.txt", L"ini");
        EXPECT_EQ(L"C:\\ABC\\def.ini", replacedPath.wstring());
    }
    {
        // 空文字
        fs::path replacedPath = PathUtils::extensionReplacedPath(L"", L"ini");
        EXPECT_EQ(L"", replacedPath.wstring());
    }
}

TEST(PathUtils, replacedPath)
{
    fs::path replacedPath = PathUtils::extensionReplacedPath(PathUtils::modulePath(nullptr), L"abc.ini");
    EXPECT_EQ(L".ini", replacedPath.extension().wstring());
    EXPECT_EQ(L"UnitTest.abc.ini", replacedPath.filename().wstring());
}
