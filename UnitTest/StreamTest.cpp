/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <gtest/gtest.h>

#include "log/Stream.h"
#include "bkzoo_log.h"

#include "debug/detect_memory_leak.h"

std::wstring stringFromFile(const std::wstring& filepath);
void removeFile(const std::wstring& filepath);

TEST(Stream, append)
{
    const std::wstring LOG_FILE_NAME(L"StreamTest.log");
    removeFile(LOG_FILE_NAME);
    auto output(std::make_unique<LogOutput>(LOG_FILE_NAME));
    auto prefix(std::make_unique<NothingPrefix>());
    unsigned char uc[] = "uc";
    void* pointer = reinterpret_cast<void*>(12345678U);
    LogStream logStream(std::move(output), std::move(prefix));
    logStream << bkzFlush;
    logStream << 1;
    logStream << 2U;
    logStream << 1L;
    logStream << 2LU;
    logStream << 4.4f;
    logStream << 3.3;
    logStream << "HELLO";
    logStream << uc;
    logStream << std::string("string");
    logStream << std::wstring(L"wstring");
    logStream << pointer;

    logStream << bkzEndl;

    const std::wstring actual = stringFromFile(LOG_FILE_NAME);
    EXPECT_EQ(L"12124.403.300HELLOucstringwstring12345678\n", actual);
}

