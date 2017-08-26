/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */


#include "Logger.h"

#include <windows.h>

#include <mutex>
#include <fstream>

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace log
    {
        std::mutex logMutex;

        LogOutput::LogOutput(const std::wstring& filepath)
            : filepath_(filepath)
        {
        }
        void LogOutput::flush(Buffer& buffer)
        {
            std::lock_guard<std::mutex> lock(::bkzoo::log::logMutex);
            std::ofstream logFile(filepath_, std::ofstream::app);
            logFile << buffer.data << std::flush;
            buffer.reset();
        }

        void OstringstreamOutput::flush(Buffer& buffer)
        {
            oss_ << buffer.data;
            str_ = oss_.str();
            oss_.str("");
            buffer.reset();
        }
        std::string OstringstreamOutput::str() const
        {
            return str_;
        }


        void LogPrefix::append(Buffer& buffer)
        {
            assert(buffer.length == 0);

            appendThreadId(buffer);
            appendDate(buffer);
            appendLogLevel(buffer);
        }
        void LogPrefix::setCurrentLogLevel(LogLevel logLevel)
        {
            currentLogLevel = logLevel;
        }
        void LogPrefix::appendThreadId(Buffer& buffer)
        {
            // threadId()とdate()とlogLevelはBufferが空のときに最初に順に書き込まれる。そのためlengthは短い想定。
            // ここで100は1024に比べれば小さいというだけの適当な数字。
            assert(buffer.length < 100);

            const int length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, "(%u) ", GetCurrentThreadId());
            if (length < 0)
            {
                assert(false); // GetCurrentThreadId() は小さいから超えないはず
                return;
            }
            buffer.length += length;
        }
        void LogPrefix::appendDate(Buffer& buffer)
        {
            // threadId()とdate()とlogLevelはBufferが空のときに最初に順に書き込まれる。そのためlengthは短い想定。
            // ここで100は1024に比べれば小さいというだけの適当な数字。
            assert(buffer.length < 100);

            const time_t t = time(nullptr);
            const size_t length = strftime(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, "{%y/%m/%d %H:%M:%S} ", localtime(&t));
            buffer.length += length;
        }
        void LogPrefix::appendLogLevel(Buffer& buffer)
        {
            // threadId()とdate()とlogLevelはBufferが空のときに最初に順に書き込まれる。そのためlengthは短い想定。
            // ここで100は1024に比べれば小さいというだけの適当な数字。
            assert(buffer.length < 100);

            constexpr char LEVEL_FORMAT[] = "[%s] ";

            int length = -1;
            switch (currentLogLevel)
            {
            case LogLevel::Non:
                length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, LEVEL_FORMAT, "Non");
                break;
            case LogLevel::Fatal:
                length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, LEVEL_FORMAT, "Fatal");
                break;
            case LogLevel::Error:
                length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, LEVEL_FORMAT, "Error");
                break;
            case LogLevel::Warning:
                length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, LEVEL_FORMAT, "Warning");
                break;
            case LogLevel::Info:
                length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, LEVEL_FORMAT, "Info");
                break;
            case LogLevel::Debug:
                length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, LEVEL_FORMAT, "Debug");
                break;
            case LogLevel::Trace:
                length = sprintf_s(buffer.data + buffer.length, Buffer::BUFFER_SIZE - buffer.length, LEVEL_FORMAT, "Trace");
                break;
            default:
                assert(false);
                break;
            }

            if (length < 0)
            {
                assert(false);
                return;
            }
            buffer.length += length;
        }

        void NothingPrefix::append(Buffer& /*buffer*/)
        {
            // do nothing
        }
        void NothingPrefix::setCurrentLogLevel(LogLevel /*logLevel*/)
        {
            // do nothing
        }

        std::unique_ptr<Logger> Logger::instance_;
        std::once_flag Logger::onceFlag_;

        Logger& Logger::instance()
        {
            assert((instance_.get() != nullptr) && "Call createInstanceCallOnceFirst() first!");
            return *instance_;
        }

        void Logger::createInstanceCallOnceFirst(const std::wstring& filepath)
        {
            std::call_once(onceFlag_, [&filepath] {
                instance_.reset(new Logger(filepath));
            });
        }



        Logger::Logger(const std::wstring& filepath, std::unique_ptr<Prefix>&& prefix)
            : maximumOutputLogLevel_(LogLevel::Error)
            , prefix_(*prefix)
            , logStream_(std::make_unique<LogOutput>(filepath), std::move(prefix))
            , nullStream_()
            , currentStream_(&logStream_)
        {
        }

        // for test
        Logger::Logger(std::unique_ptr<Output>&& output, std::unique_ptr<Prefix>&& prefix)
            : maximumOutputLogLevel_(LogLevel::Error)
            , prefix_(*prefix)
            , logStream_(std::move(output), std::move(prefix))
            , nullStream_()
            , currentStream_(&logStream_)
        {
        }

        void Logger::setMaximumOutputLogLevel(LogLevel logLevel)
        {
            maximumOutputLogLevel_ = logLevel;
        }

    }
}

