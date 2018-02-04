/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <gtest/gtest.h>

#include "bkzoo_log.h"

#include <fstream>
#include <thread>
#include <regex>
#include <random>

#include <filesystem>
using namespace std::experimental;

#include <Windows.h>

#include "for_debug/detect_memory_leak.h"

std::wstring stringFromFile(const std::wstring& filepath)
{
    const std::wifstream wif(filepath);
    std::wostringstream buf;
    buf << wif.rdbuf();
    return buf.str();
}
std::list<std::wstring> linesFromFile(const std::wstring& filepath)
{
    std::list<std::wstring> lines;
    std::wifstream wif(filepath);
    std::wstring line;
    while (std::getline(wif, line))
    {
        lines.push_back(line);
    }
    return lines;
}

void removeFile(const std::wstring& filepath)
{
    filesystem::path p(filepath);
    try
    {
        filesystem::remove(p);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << " " << p << "を削除できません。別プロセスで書き込み中かも。";
        throw;
    }
}


namespace bkzoo
{
    namespace log
    {
        TEST(LoggerTest, instance)
        {
            const std::wstring LOG_FILE_NAME(L"LoggerTest_instance.log");
            removeFile(LOG_FILE_NAME);
            Logger::createInstanceCallOnceFirst(LOG_FILE_NAME);

            {
                removeFile(LOG_FILE_NAME);
                Logger::instance().setMaximumOutputLogLevel(LogLevel::Fatal);
                Logger::instance() << LogLevel::Error << "HELLO";
                EXPECT_FALSE(filesystem::exists(LOG_FILE_NAME));
            }

            {
                removeFile(LOG_FILE_NAME);
                Logger::instance().setMaximumOutputLogLevel(LogLevel::Error);
                Logger::instance() << LogLevel::Error << "HELLO ";
                Logger::instance() << LogLevel::Error << "HELLO" << bkzEndl;
                EXPECT_TRUE(filesystem::exists(LOG_FILE_NAME));
                const std::wstring actual = stringFromFile(LOG_FILE_NAME);
                EXPECT_TRUE(actual.find(L"HELLO HELLO\n") != std::string::npos);
            }

            Logger::instance().setMaximumOutputLogLevel(LogLevel::Info);
            {
                removeFile(LOG_FILE_NAME);
                LOG_FATAL << "Fatal";
                EXPECT_TRUE(filesystem::exists(LOG_FILE_NAME));
                const std::wstring actual = stringFromFile(LOG_FILE_NAME);
                EXPECT_TRUE(actual.find(L"Fatal\n") != std::string::npos);
            }
            {
                removeFile(LOG_FILE_NAME);
                LOG_ERROR << "Error";
                EXPECT_TRUE(filesystem::exists(LOG_FILE_NAME));
                const std::wstring actual = stringFromFile(LOG_FILE_NAME);
                EXPECT_TRUE(actual.find(L"Error\n") != std::string::npos);
            }
            {
                removeFile(LOG_FILE_NAME);
                LOG_WARNING << "Warning";
                EXPECT_TRUE(filesystem::exists(LOG_FILE_NAME));
                const std::wstring actual = stringFromFile(LOG_FILE_NAME);
                EXPECT_TRUE(actual.find(L"Warning\n") != std::string::npos);
            }
            {
                removeFile(LOG_FILE_NAME);
                LOG_INFO << "Info";
                EXPECT_TRUE(filesystem::exists(LOG_FILE_NAME));
                const std::wstring actual = stringFromFile(LOG_FILE_NAME);
                EXPECT_TRUE(actual.find(L"Info\n") != std::string::npos);
            }
            {
                removeFile(LOG_FILE_NAME);
                LOG_DEBUG << "Debug";
                EXPECT_FALSE(filesystem::exists(LOG_FILE_NAME));
            }
            {
                removeFile(LOG_FILE_NAME);
                LOG_TRACE << "Trace";
                EXPECT_FALSE(filesystem::exists(LOG_FILE_NAME));
            }

            // ログ・ファイル確認用に同じテスト実施。
            {
                removeFile(LOG_FILE_NAME);
                LOG_ERROR << "Error2";
                EXPECT_TRUE(filesystem::exists(LOG_FILE_NAME));
                const std::wstring actual = stringFromFile(LOG_FILE_NAME);
                EXPECT_TRUE(actual.find(L"Error2\n") != std::string::npos);
            }
        }

        // cppcheck-suppress syntaxError
        TEST(LoggerTest, logger)
        {
            const std::wstring LOG_FILE_NAME(L"LoggerTest.log");
            removeFile(LOG_FILE_NAME);
            Logger logger(LOG_FILE_NAME);
            logger.setMaximumOutputLogLevel(LogLevel::Error);
            logger << LogLevel::Error;
            logger << bkzFlush;
            logger << 1;
            logger << 2U;
            logger << 3.3;
            logger << 4.4f;
            logger << "HELLO";
            logger << bkzEndl;

            const std::wstring actual = stringFromFile(LOG_FILE_NAME);
            std::wcout << actual;
            const std::wregex re(L"^\\([0-9a-fA-F]+\\) \\{[ 0-9/:]+\\} \\[Error\\] .*\n$");
            EXPECT_TRUE(std::regex_match(actual, re));
            EXPECT_TRUE(actual.find(L"123.3004.40HELLO\n") != std::string::npos);
        }

        TEST(LoggerTest, thread)
        {
            const std::wstring LOG_FILE_NAME(L"LoggerTest_thread.log");
            removeFile(LOG_FILE_NAME);
            Logger logger(std::make_unique<LogOutput>(LOG_FILE_NAME), std::make_unique<NothingPrefix>());
            std::vector<std::thread> threads;
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> thousand(1, 10);

            constexpr int TEST_SIZE = 1000;
            for (int i = 0; i < TEST_SIZE; ++i)
            {
                threads.push_back(std::thread([&thousand, &mt, &logger, i] { Sleep(thousand(mt)); logger << i << bkzEndl; }));
            }
            for (auto& t : threads)
            {
                t.join();
            }

            std::list<std::wstring> lines = linesFromFile(LOG_FILE_NAME);
            EXPECT_EQ(TEST_SIZE, lines.size());

            int i = 0;
            bool equalAll = true;
            for (const auto& line : lines)
            {
                if (std::to_wstring(i++) != line)
                {
                    equalAll = false;
                    break;
                }
            }
            EXPECT_FALSE(equalAll); // 非常にまれに一致することはあるが。

            i = 0;
            lines.sort([](const std::wstring& a, const std::wstring& b) {
                return std::stoi(a) < std::stoi(b);
            });
            for (const auto& line : lines)
            {
                const std::wstring expected(std::to_wstring(i++));
                EXPECT_EQ(expected, line);
            }

        }

    } // namespace log
} // for namespace bkzoo


TEST(LogPrefixTest, threadId)
{
    Buffer buffer;
    LogPrefix::appendThreadId(buffer);
    std::cout << "threadId=" << buffer.data << std::endl;
    const std::regex re("^\\([0-9a-fA-F]+\\) $");
    EXPECT_TRUE(std::regex_match(buffer.data, re));
}

TEST(LogPrefixTest, date)
{
    //    std::locale::global(std::locale::classic());
    //    std::locale::global(std::locale("ja"));
    Buffer buffer;
    LogPrefix::appendDate(buffer);
    std::cout << "date=" << buffer.data << std::endl;
    const std::regex re("^\\{[0-9]{2}/[0-9]{2}/[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}\\} $");
    EXPECT_TRUE(std::regex_match(buffer.data, re));
}

TEST(LogPrefixTest, logLevel)
{
    LogPrefix logPrefix;
    {
        Buffer buffer;
        logPrefix.currentLogLevel = LogLevel::Debug;
        logPrefix.appendLogLevel(buffer);
        std::cout << "logLevel=" << buffer.data << std::endl;
        const std::regex re("^\\[Debug\\] $");
        EXPECT_TRUE(std::regex_match(buffer.data, re));
    }
    {
        Buffer buffer;
        logPrefix.currentLogLevel = LogLevel::Error;
        logPrefix.appendLogLevel(buffer);
        std::cout << "logLevel=" << buffer.data << std::endl;
        const std::regex re("^\\[Error\\] $");
        EXPECT_TRUE(std::regex_match(buffer.data, re));
    }
    {
        Buffer buffer;
        logPrefix.currentLogLevel = LogLevel::Fatal;
        logPrefix.appendLogLevel(buffer);
        std::cout << "logLevel=" << buffer.data << std::endl;
        const std::regex re("^\\[Fatal\\] $");
        EXPECT_TRUE(std::regex_match(buffer.data, re));
    }
    {
        Buffer buffer;
        logPrefix.currentLogLevel = LogLevel::Info;
        logPrefix.appendLogLevel(buffer);
        std::cout << "logLevel=" << buffer.data << std::endl;
        const std::regex re("^\\[Info\\] $");
        EXPECT_TRUE(std::regex_match(buffer.data, re));
    }
    {
        Buffer buffer;
        logPrefix.currentLogLevel = LogLevel::Non;
        logPrefix.appendLogLevel(buffer);
        std::cout << "logLevel=" << buffer.data << std::endl;
        const std::regex re("^\\[Non\\] $");
        EXPECT_TRUE(std::regex_match(buffer.data, re));
    }
    {
        Buffer buffer;
        logPrefix.currentLogLevel = LogLevel::Trace;
        logPrefix.appendLogLevel(buffer);
        std::cout << "logLevel=" << buffer.data << std::endl;
        const std::regex re("^\\[Trace\\] $");
        EXPECT_TRUE(std::regex_match(buffer.data, re));
    }
    {
        Buffer buffer;
        logPrefix.currentLogLevel = LogLevel::Warning;
        logPrefix.appendLogLevel(buffer);
        std::cout << "logLevel=" << buffer.data << std::endl;
        const std::regex re("^\\[Warning\\] $");
        EXPECT_TRUE(std::regex_match(buffer.data, re));
    }
}
