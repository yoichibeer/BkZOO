/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */


#include "Stream.h"

#include "bkzoo_string.h"

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace log
    {

        // cppcheck-suppress unusedFunction   bkzoo_log.h のマクロで使用している。
        Stream& bkzEndl(Stream& stream)
        {
            stream.endl();
            return stream;
        }

        // cppcheck-suppress unusedFunction   bkzoo_log.h のマクロで使用している。
        Stream& bkzFlush(Stream& stream)
        {
            stream.flush();
            return stream;
        }


        Stream::~Stream() = default;


        thread_local Buffer LogStream::buffer_thl_;

        LogStream::LogStream(std::unique_ptr<Output>&& output, std::unique_ptr<Prefix>&& prefix)
            : stringBuffer_(std::move(output), std::move(prefix))
        {
        }

        Stream& LogStream::operator<<(int val)
        {
            buffer_thl_.reset();
            buffer_thl_.length = ::sprintf_s<Buffer::BUFFER_SIZE>(buffer_thl_.data, "%d", val);
            if (buffer_thl_.length == -1)
            {
                assert(false); // intの大きさからしてここはこないはず。
                return *this;
            }
            stringBuffer_.append(buffer_thl_);
            return *this;
        }
        Stream& LogStream::operator<<(unsigned int val)
        {
            buffer_thl_.reset();
            buffer_thl_.length = ::sprintf_s<Buffer::BUFFER_SIZE>(buffer_thl_.data, "%u", val);
            if (buffer_thl_.length == -1)
            {
                assert(false);
                return *this;
            }
            stringBuffer_.append(buffer_thl_);
            return *this;
        }
        Stream& LogStream::operator<<(long val)
        {
            *this << static_cast<int>(val);
            return *this;
        }
        Stream& LogStream::operator<<(unsigned long val)
        {
            *this << static_cast<unsigned int>(val);
            return *this;
        }
        Stream& LogStream::operator<<(float val)
        {
            buffer_thl_.reset();
            buffer_thl_.length = ::sprintf_s<Buffer::BUFFER_SIZE>(buffer_thl_.data, "%.2f", val);
            if (buffer_thl_.length == -1)
            {
                assert(false);
                return *this;
            }
            stringBuffer_.append(buffer_thl_);
            return *this;
        }
        Stream& LogStream::operator<<(double val)
        {
            buffer_thl_.reset();
            buffer_thl_.length = ::sprintf_s<Buffer::BUFFER_SIZE>(buffer_thl_.data, "%.3lf", val);
            if (buffer_thl_.length == -1)
            {
                assert(false);
                return *this;
            }
            assert(buffer_thl_.length < Buffer::BUFFER_SIZE);
            stringBuffer_.append(buffer_thl_);
            return *this;
        }
        Stream& LogStream::appendString(const char* start, size_t length)
        {
            do
            {
                if (length == 0)
                    break;

                buffer_thl_.reset();
                const size_t copyingLength = Buffer::BUFFER_SIZE - 1 < length ? Buffer::BUFFER_SIZE - 1 : length;
                ::strncpy_s(buffer_thl_.data, Buffer::BUFFER_SIZE, start, copyingLength);
                buffer_thl_.length = copyingLength;

                stringBuffer_.append(buffer_thl_);
                start += buffer_thl_.length;
                length -= buffer_thl_.length;
            } while (true);
            return *this;
        }
        Stream& LogStream::operator<<(const char* val)
        {
            constexpr size_t MAX_LEN = USHRT_MAX/*65535*/;
            size_t len = strnlen(val, MAX_LEN);
            if (len == MAX_LEN)
                return *this;

            const char* start = val;
            return appendString(start, len);
        }
        Stream& LogStream::operator<<(const unsigned char* val)
        {
            const char* valString = reinterpret_cast<const char*>(val);
            *this << valString;
            return *this;
        }
        Stream& LogStream::operator<<(const std::string& val)
        {
            const char* start = val.c_str();
            return appendString(start, val.length());
        }
        Stream& LogStream::operator<<(const std::wstring& val)
        {
            /// @todo wchar_t のログ出力するときに、メモリ確保してる。細かく変換すればnewしなくていいはず。
            *this << wm_cast<std::string>(val);
            return *this;
        }
        Stream& LogStream::operator<<(void* val)
        {
            *this << reinterpret_cast<size_t>(val);
            return *this;
        }



        Stream& LogStream::operator<<(Stream& (*fn)(Stream&))
        {
            return ((*fn)(*this));
        }
        Stream& LogStream::endl()
        {
            stringBuffer_.append({ 1, "\n" });
            flush();
            return *this;
        }
        Stream& LogStream::flush()
        {
            stringBuffer_.flush();
            return *this;
        }



        Stream& NullStream::operator<<(int)
        {
            return *this;
        }
        Stream& NullStream::operator<<(unsigned int)
        {
            return *this;
        }
        Stream& NullStream::operator<<(long)
        {
            return *this;
        }
        Stream& NullStream::operator<<(unsigned long)
        {
            return *this;
        }
        Stream& NullStream::operator<<(float)
        {
            return *this;
        }
        Stream& NullStream::operator<<(double)
        {
            return *this;
        }
        Stream& NullStream::operator<<(const char*)
        {
            return *this;
        }
        Stream& NullStream::operator<<(const unsigned char*)
        {
            return *this;
        }
        Stream& NullStream::operator<<(const std::string&)
        {
            return *this;
        }
        Stream& NullStream::operator<<(const std::wstring&)
        {
            return *this;
        }
        Stream& NullStream::operator<<(void*)
        {
            return *this;
        }

        Stream & NullStream::operator<<(Stream &(*)(Stream &stream))
        {
            return *this;
        }

        Stream& NullStream::endl()
        {
            return *this;
        }

        Stream& NullStream::flush()
        {
            return *this;
        }

    }
}

