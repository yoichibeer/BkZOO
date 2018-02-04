/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */


#include "StringBuffer.h"

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace log
    {

        void Buffer::reset()
        {
            *this = Buffer();
        }


        Output::~Output() = default;
        Prefix::~Prefix() = default;


        thread_local Buffer StringBuffer::buffer_thl_;


        StringBuffer::StringBuffer(std::unique_ptr<Output>&& output, std::unique_ptr<Prefix>&& prefix)
            : output_(std::move(output)), prefix_(std::move(prefix))
        {
        }

        void StringBuffer::appendPrefix(Buffer& buffer)
        {
            assert(buffer.length == 0);
            prefix_->append(buffer);
        }

        void StringBuffer::append(const Buffer& rhs)
        {
            if (buffer_thl_.length == 0)
            {
                appendPrefix(buffer_thl_);
            }

            assert(buffer_thl_.length < Buffer::BUFFER_SIZE);
            const size_t capacity = Buffer::BUFFER_SIZE - 1 - buffer_thl_.length;
            if (capacity >= rhs.length)
            {
                // キャパを超えない場合、rhsすべてを書き込みコピー。flushまでしないで抜ける。
                ::strncpy_s(buffer_thl_.data + buffer_thl_.length, capacity + 1, rhs.data, rhs.length);
                buffer_thl_.length += rhs.length;
                return;
            }

            // キャパを超える場合
            assert(capacity < rhs.length);

            // 残りのcapacity分を目一杯書き込みコピー
            ::strncpy_s(buffer_thl_.data + buffer_thl_.length, capacity + 1, rhs.data, capacity);
            buffer_thl_.length = Buffer::BUFFER_SIZE - 1;

            // いっぱいにしたので書き込み。書き込み後はからっぽに。
            flush();

            // 上で書き込みコピーできなかったの残りの文字列を再帰呼び出しでappend()
            Buffer reminder;
            assert(capacity < rhs.length);
            const size_t reminderLength = rhs.length - capacity;
            ::strncpy_s(reminder.data, Buffer::BUFFER_SIZE, rhs.data + capacity, reminderLength);
            reminder.length = reminderLength;
            append(reminder); // 再起呼び出しは一回だけのはず。
        }

        void StringBuffer::flush()
        {
            output_->flush(buffer_thl_);
        }

    }
}
