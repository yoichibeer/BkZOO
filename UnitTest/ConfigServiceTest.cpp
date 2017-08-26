#include <gtest/gtest.h>

#include "config/ConfigService.h"
#include "config/Config.h"

#include "util/PathUtils.h"

#include "debug/detect_memory_leak.h"

using namespace bkzoo;

class ConfigServiceTest : public ::testing::Test
{
protected:
    // cppcheck-suppress unusedFunction
    virtual void SetUp() override
    {
        const fs::path modulePath = util::PathUtils::modulePath(nullptr);
        standardPath_ = util::PathUtils::extensionReplacedPath(modulePath, L"ini");
        fs::copy(L"../UnitTest/testdata/BkZOO.ini", standardPath_);

        presetDirPath_ = modulePath.parent_path() / modulePath.stem();
        fs::create_directory(presetDirPath_);
        const fs::path presetPath = presetDirPath_ / fs::path(modulePath).replace_extension(L"preset.ini").filename();
        fs::copy(L"../UnitTest/testdata/BkZOO.preset.ini", presetPath);
    }

    // cppcheck-suppress unusedFunction
    virtual void TearDown() override
    {
        fs::remove(standardPath_);
        fs::remove_all(presetDirPath_);
    }

private:
    fs::path standardPath_;
    fs::path presetDirPath_;
};

TEST_F(ConfigServiceTest, initialize)
{
    EXPECT_EQ(0U, config::Config::instance().sites().size());

    bool ret = config::ConfigService::initialize(nullptr);
    ASSERT_TRUE(ret);

    EXPECT_EQ(23, config::Config::instance().sites().size());
}
