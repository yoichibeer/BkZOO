#include <gtest/gtest.h>

#include "util/BeckyVersion.h"

#include "bkzoo_config.h"
#include "config\ConfigService.h"

using namespace bkzoo::util;

TEST(BeckyVersionTest, extractBeckyVersionFrom)
{
    // Success
    EXPECT_EQ("2.72.01", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.72.01 [ja]"));
    EXPECT_EQ("2.72.01", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.72.01 [en]"));
    EXPECT_EQ("2.720.051", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.720.051 [ja]"));
    EXPECT_EQ("02.72.01", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 02.72.01 [en-gb]"));
    EXPECT_EQ("2.73", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.73 [ja]"));

    // Failed
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("ecky! ver. 2.72.01 [ja]"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.72.01.99 [ja]"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky! ver 2.72.01 [ja]"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky ver. 2.72.01 [ja]"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.72.01 []"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.72.01 ja]"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.72.01 [ja"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky! ver.2.72.01 [ja]"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky! ver. 2.72.01[ja]"));
    EXPECT_EQ("", BeckyVersion::extractBeckyVersionFrom("Becky!ver. 2.72.01 [ja]"));
}

TEST(BeckyVersionTest, isValidBeckyVersion)
{
    // Success
    EXPECT_TRUE(BeckyVersion::isValidBeckyVersion("2.72.01"));
    EXPECT_TRUE(BeckyVersion::isValidBeckyVersion("02.720.010"));
    EXPECT_TRUE(BeckyVersion::isValidBeckyVersion("2.73"));

    // Failed
    EXPECT_FALSE(BeckyVersion::isValidBeckyVersion("2.72.01 "));
    EXPECT_FALSE(BeckyVersion::isValidBeckyVersion("2.72. 01"));
    EXPECT_FALSE(BeckyVersion::isValidBeckyVersion(" 2.72.01"));
    EXPECT_FALSE(BeckyVersion::isValidBeckyVersion("2.72."));
    EXPECT_FALSE(BeckyVersion::isValidBeckyVersion("2.72.a"));
    EXPECT_FALSE(BeckyVersion::isValidBeckyVersion("2.72a.01"));
    EXPECT_FALSE(BeckyVersion::isValidBeckyVersion("2.72.01.01"));
}

TEST(BeckyVersionTest, getBeckyVersionOfMyself)
{
    // Success
    BeckyVersion beckyVersion;
    beckyVersion.initialize("2.72.01");
    EXPECT_EQ(beckyVersion, BeckyVersion::getBeckyVersionOfMyself("2.72.01"));
}

TEST(BeckyVersionTest, getBeckyVersionLatest)
{
    ConfigService::initialize(nullptr);
    Config::instance().setVersion(L"2.73");
    // Success
    BeckyVersion beckyVersion;
    beckyVersion.initialize("2.73");
    EXPECT_EQ(beckyVersion, BeckyVersion::getBeckyVersionLatest("2.72.01"));
}


// for private member test
namespace bkzoo
{
    namespace util
    {

        static BeckyVersion getBeckyVersion()
        {
            BeckyVersion beckyVersion;
            beckyVersion.initialize("1.2.3");
            return std::move(beckyVersion);
        }

        // cppcheck-suppress syntaxError
        TEST(BeckyVersionTest, moveConstructor)
        {
            BeckyVersion beckyVersion(getBeckyVersion());
            EXPECT_EQ("1.2.3", beckyVersion.version());
            EXPECT_EQ(1, beckyVersion.versions_[0]);
            EXPECT_EQ(2, beckyVersion.versions_[1]);
            EXPECT_EQ(3, beckyVersion.versions_[2]);
#ifndef NDEBUG
            EXPECT_TRUE(beckyVersion.isValid_);
#endif // NDEBUG
        }

        TEST(BeckyVersionTest, moveAssignmentOperator)
        {
            BeckyVersion beckyVersion = getBeckyVersion();
            EXPECT_EQ("1.2.3", beckyVersion.version());
            EXPECT_EQ(1, beckyVersion.versions_[0]);
            EXPECT_EQ(2, beckyVersion.versions_[1]);
            EXPECT_EQ(3, beckyVersion.versions_[2]);
#ifndef NDEBUG
            EXPECT_TRUE(beckyVersion.isValid_);
#endif // NDEBUG
        }

        TEST(BeckyVersionTest, isValid)
        {
            BeckyVersion beckyVersion;
#ifndef NDEBUG
            EXPECT_FALSE(beckyVersion.isValid_);
#endif // NDEBUG
            beckyVersion.initialize("1.2.3");
#ifndef NDEBUG
            EXPECT_TRUE(beckyVersion.isValid_);
#endif // NDEBUG
        }

    }
}


TEST(BeckyVersionTest, operatorLess)
{
    // Success
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.4");
        EXPECT_TRUE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.3.2");
        EXPECT_TRUE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("02.01.010");
        EXPECT_TRUE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.02.030");
        BeckyVersion rhs;
        rhs.initialize("1.2.31");
        EXPECT_TRUE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("2.72.01");
        BeckyVersion rhs;
        rhs.initialize("2.73");
        EXPECT_TRUE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("2.72");
        BeckyVersion rhs;
        rhs.initialize("2.73.01");
        EXPECT_TRUE(lhs < rhs);
    }

    // Failed
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_FALSE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.03");
        BeckyVersion rhs;
        rhs.initialize("01.02.3");
        EXPECT_FALSE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.2");
        EXPECT_FALSE(lhs < rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.02.03");
        BeckyVersion rhs;
        rhs.initialize("1.02.02");
        EXPECT_FALSE(lhs < rhs);
    }
}

TEST(BeckyVersionTest, operatorGreater)
{
    // Success
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.4");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs > rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.3.2");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs > rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("02.01.010");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs > rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.31");
        BeckyVersion rhs;
        rhs.initialize("01.02.030");
        EXPECT_TRUE(lhs > rhs);
    }

    // Failed
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_FALSE(lhs > rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.02.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.03");
        EXPECT_FALSE(lhs > rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.2");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_FALSE(lhs > rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.02.02");
        BeckyVersion rhs;
        rhs.initialize("01.02.03");
        EXPECT_FALSE(lhs > rhs);
    }
}

TEST(BeckyVersionTest, operatorLessOrEqual)
{
    // Success
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.4");
        EXPECT_TRUE(lhs <= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.3.2");
        EXPECT_TRUE(lhs <= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("02.01.010");
        EXPECT_TRUE(lhs <= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.02.030");
        BeckyVersion rhs;
        rhs.initialize("1.2.31");
        EXPECT_TRUE(lhs <= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs <= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.03");
        BeckyVersion rhs;
        rhs.initialize("01.02.3");
        EXPECT_TRUE(lhs <= rhs);
    }

    // Failed
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.2");
        EXPECT_FALSE(lhs <= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.02.03");
        BeckyVersion rhs;
        rhs.initialize("1.02.02");
        EXPECT_FALSE(lhs <= rhs);
    }
}

TEST(BeckyVersionTest, operatorGreaterOrEqual)
{
    // Success
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.4");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs >= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.3.2");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs >= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("02.01.010");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs >= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.31");
        BeckyVersion rhs;
        rhs.initialize("01.02.030");
        EXPECT_TRUE(lhs >= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs >= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.03");
        BeckyVersion rhs;
        rhs.initialize("01.02.3");
        EXPECT_TRUE(lhs >= rhs);
    }

    // Failed
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.2");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_FALSE(lhs >= rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("1.02.02");
        BeckyVersion rhs;
        rhs.initialize("01.02.03");
        EXPECT_FALSE(lhs >= rhs);
    }
}

TEST(BeckyVersionTest, operatorEqual)
{
    // Success
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_TRUE(lhs == rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.02.003");
        EXPECT_TRUE(lhs == rhs);
    }

    // Failed
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.2");
        EXPECT_FALSE(lhs == rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.02.03");
        BeckyVersion rhs;
        rhs.initialize("1.02.02");
        EXPECT_FALSE(lhs == rhs);
    }
}

TEST(BeckyVersionTest, operatorNotEqual)
{
    // Success
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.2");
        EXPECT_TRUE(lhs != rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.02.03");
        BeckyVersion rhs;
        rhs.initialize("1.02.02");
        EXPECT_TRUE(lhs != rhs);
    }

    // Failed
    {
        BeckyVersion lhs;
        lhs.initialize("1.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.2.3");
        EXPECT_FALSE(lhs != rhs);
    }
    {
        BeckyVersion lhs;
        lhs.initialize("01.2.3");
        BeckyVersion rhs;
        rhs.initialize("1.02.003");
        EXPECT_FALSE(lhs != rhs);
    }
}
