/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_LOG_STREAM_H
#define BKZ_LOG_STREAM_H

#include "StringBuffer.h"

#include "defs.h"

#include <string>

namespace bkzoo
{
    namespace log
    {

        class Stream
        {
        public:
            Stream() = default;
            virtual ~Stream() = 0;

            virtual Stream& operator<<(int val) = 0;
            virtual Stream& operator<<(unsigned int val) = 0;
            virtual Stream& operator<<(float val) = 0;
            virtual Stream& operator<<(double val) = 0;
            virtual Stream& operator<<(const char* val) = 0;
            virtual Stream& operator<<(const unsigned char* val) = 0;
            virtual Stream& operator<<(const std::string& val) = 0;
            virtual Stream& operator<<(const std::wstring& val) = 0;
            virtual Stream& operator<<(void* val) = 0;

            virtual Stream& operator<<(Stream& (*fn)(Stream& stream)) = 0;

            virtual Stream& endl() = 0;
            virtual Stream& flush() = 0;

        private:
            DISALLOW_COPY_AND_ASSIGN(Stream);
        };

        Stream& bkzEndl(Stream& stream);

        Stream& bkzFlush(Stream& stream);

        class LogStream final : public Stream
        {
        public:
            LogStream(std::unique_ptr<Output>&& output, std::unique_ptr<Prefix>&& prefix);
            virtual ~LogStream() = default;

            virtual Stream& operator<<(int val) override;
            virtual Stream& operator<<(unsigned int val) override;
            virtual Stream& operator<<(float val) override;
            virtual Stream& operator<<(double val) override;
            virtual Stream& operator<<(const char* val) override;
            virtual Stream& operator<<(const unsigned char* val) override;
            virtual Stream& operator<<(const std::string& val) override;
            virtual Stream& operator<<(const std::wstring& val) override;
            virtual Stream& operator<<(void* val) override;

            virtual Stream& operator<<(Stream& (*fn)(Stream&)) override;
            virtual Stream& endl() override;
            virtual Stream& flush() override;

        private:
            Stream& appendString(const char* start, size_t length);

            StringBuffer stringBuffer_;
            static thread_local Buffer buffer_thl_;

        private:
            DISALLOW_COPY_AND_ASSIGN(LogStream);
        };

        class NullStream final : public Stream
        {
        public:
            NullStream() = default;
            virtual ~NullStream() = default;

            virtual Stream& operator<<(int val) override;
            virtual Stream& operator<<(unsigned int val) override;
            virtual Stream& operator<<(float val) override;
            virtual Stream& operator<<(double val) override;
            virtual Stream& operator<<(const char* val) override;
            virtual Stream& operator<<(const unsigned char* val) override;
            virtual Stream& operator<<(const std::string& val) override;
            virtual Stream& operator<<(const std::wstring& val) override;
            virtual Stream& operator<<(void* val) override;

            virtual Stream& operator<<(Stream& (*)(Stream& stream)) override;
            virtual Stream& endl() override;
            virtual Stream& flush() override;

        private:
            DISALLOW_COPY_AND_ASSIGN(NullStream);
        };

    }
}

#endif // BKZ_LOG_STREAM_H
