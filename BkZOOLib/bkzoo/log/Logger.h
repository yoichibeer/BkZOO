/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_LOG_LOGGER_H
#define BKZ_LOG_LOGGER_H

#include "Stream.h"
#include "LogLevel.h"

#include "defs.h"

#include <string>
#include <sstream>
#include <mutex>

#include "bkzoo_gtest.h"

namespace bkzoo
{
    namespace log
    {

        class LogOutput final : public Output
        {
        public:
            explicit LogOutput(const std::wstring& filepath);
            virtual void flush(Buffer& buffer) override;
        private:
            const std::wstring filepath_;
        };

        // for test purpose
        class OstringstreamOutput final : public Output
        {
        public:
            OstringstreamOutput() = default;
            virtual ~OstringstreamOutput() = default;

            virtual void flush(Buffer& buffer) override;
            std::string str() const;

        private:
            std::ostringstream oss_;
            std::string str_;
        };


        class LogPrefix final : public Prefix
        {
        public:
            LogPrefix() = default;
            virtual ~LogPrefix() = default;
            virtual void append(Buffer& buffer) override;
            virtual void setCurrentLogLevel(LogLevel logLevel) override;

            static void appendThreadId(Buffer& buffer);
            static void appendDate(Buffer& buffer);
            void appendLogLevel(Buffer& buffer);
            LogLevel currentLogLevel = LogLevel::Non;
        };

        // for test purpose
        class NothingPrefix final : public Prefix
        {
        public:
            NothingPrefix() = default;
            virtual ~NothingPrefix() = default;

            virtual void append(Buffer& buffer) override;
            virtual void setCurrentLogLevel(LogLevel logLevel) override;
        };


        class Logger final
        {
            friend struct std::default_delete<Logger>;
            FRIEND_GTEST(LoggerTest, logger);
            FRIEND_GTEST(LoggerTest, thread);
        public:

            // createInstanceCallOnceFirst() を先に呼ばないと instance() を呼んではいけない！
            static Logger& instance();
            static void createInstanceCallOnceFirst(const std::wstring& filepath);

            void setMaximumOutputLogLevel(LogLevel logLevel);

            template<typename U>
            Logger& operator<<(U u);

            template<>
            Logger& operator<<(LogLevel t);

            /// @note (yoichi) 2017/08/26 friend struct std::default_delete<Logger>; すれば下記は不要！
            //struct deleter
            //{
            //    void operator()(Logger const* const p) const
            //    {
            //        delete p;
            //    }
            //};

        private:
            LogLevel maximumOutputLogLevel_;
            Prefix& prefix_;

            /// @note (yoichi) 2017/08/18 DebugStreamやStdoutStreamなど追加可能。混合Streamも可能。
            LogStream logStream_;
            NullStream nullStream_;
            Stream* currentStream_;

            // シングルトン用の部品
            static std::unique_ptr<Logger> instance_;
            static std::once_flag onceFlag_;

        private:
            explicit Logger(const std::wstring& filepath, std::unique_ptr<Prefix>&& prefix = std::make_unique<LogPrefix>());

            // for test purpose
            Logger(std::unique_ptr<Output>&& output, std::unique_ptr<Prefix>&& prefix);

            ~Logger() = default;

            DISALLOW_COPY_AND_ASSIGN(Logger);
        };

        template<typename U>
        Logger& Logger::operator<<(U u)
        {
            *currentStream_ << u;
            return *this;
        }

        template<>
        Logger& Logger::operator<<(LogLevel currentLogLevel)
        {
            if (maximumOutputLogLevel_ < currentLogLevel)
            {
                currentStream_ = &nullStream_;
            }
            else
            {
                currentStream_ = &logStream_;
                prefix_.setCurrentLogLevel(currentLogLevel);
            }

            return *this;
        }


    }


}

#endif // BKZ_LOG_LOGGER_H
