/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <gtest/gtest.h>

#include "bkzoo_log.h"

#include "debug/detect_memory_leak.h"

using namespace bkzoo::log;

TEST(StreamBuffer, append)
{
    OstringstreamOutput oss;
    Buffer buffer = { 10, "0123456789" };
    oss.flush(buffer);
    EXPECT_EQ("0123456789", oss.str());

    std::unique_ptr<OstringstreamOutput> ossoutput = std::make_unique<OstringstreamOutput>();
    OstringstreamOutput* pOssoutput = ossoutput.get();
    std::unique_ptr<NothingPrefix> nothingPrefix = std::make_unique<NothingPrefix>();
    StringBuffer stringBuffer(std::move(ossoutput), std::move(nothingPrefix));

#define DATA1 "Hello, StringBuffer!"
    size_t length1 = strlen(DATA1);
    Buffer buffer1 = { length1, DATA1 };
    stringBuffer.append(buffer1);

#define DATA2 " How are you?"
    size_t length2 = strlen(DATA2);
    Buffer buffer2 = { length2, DATA2 };
    stringBuffer.append(buffer2);

    EXPECT_EQ("", pOssoutput->str());

    stringBuffer.flush();

    EXPECT_EQ("Hello, StringBuffer! How are you?", pOssoutput->str());
}

TEST(StreamBuffer, append_1024)
{
#define DATA3 "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789" \
        "01234567890123456789012"

    OstringstreamOutput oss;
    Buffer buffer = { 1024, DATA3 };
    EXPECT_EQ("", oss.str());

    oss.flush(buffer);
    EXPECT_EQ(DATA3, oss.str());

    std::unique_ptr<OstringstreamOutput> ossoutput = std::make_unique<OstringstreamOutput>();
    std::unique_ptr<NothingPrefix> nothingPrefix = std::make_unique<NothingPrefix>();
    OstringstreamOutput* pOssoutput = ossoutput.get();
    StringBuffer stringBuffer(std::move(ossoutput), std::move(nothingPrefix));

    size_t length1 = strlen(DATA3);
    Buffer buffer1 = { length1, DATA3 };
    stringBuffer.append(buffer1);
    EXPECT_EQ("", pOssoutput->str());

#define DATA4 "How are you?"
    size_t length2 = strlen(DATA4);
    Buffer buffer2 = { length2, DATA4 };
    stringBuffer.append(buffer2);
    EXPECT_EQ(DATA3, pOssoutput->str());

    stringBuffer.flush();

    EXPECT_EQ("How are you?", pOssoutput->str());
}
