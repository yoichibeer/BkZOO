/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_LOG_STRINGBUFFER_H
#define BKZ_LOG_STRINGBUFFER_H

#include "defs.h"
#include "LogLevel.h"

#include <memory>

namespace bkzoo
{
    namespace log
    {

        /**
         * 文字列バッファ。
         * ログ出力時にnewできない場合でも出力できるようにこのバッファを使う（wchar_t出力でnewする処理がのこっている2017/08/18)。
         * thread_localで使われることを想定。
         */
        struct Buffer
        {
            static const size_t BUFFER_SIZE = 1024U;
            size_t length = 0;
            char data[BUFFER_SIZE] = { 0 }; // null終端分をここで余分に確保する
            void reset();
        };

        /**
         * 出力先インターフェース。
         * このインターフェースを継承してログ出力するクラスと、テスト用にメモリ（StringStream）に出力するクラスをつくる。
         */
        class Output
        {
        public:
            Output() = default;
            virtual ~Output() = 0;
            virtual void flush(Buffer& buffer) = 0;
        private:
            DISALLOW_COPY_AND_ASSIGN(Output);
        };


        /**
         * ログの最初に出力するPrefixインターフェース。
         * このインターフェースを継承してログ出力するクラスと、テスト用に何も出力しないクラスを作成する。
         */
        class Prefix
        {
        public:
            Prefix() = default;
            virtual ~Prefix() = 0;
            virtual void append(Buffer& buffer) = 0;
            virtual void setCurrentLogLevel(LogLevel logLevel) = 0;
        private:
            DISALLOW_COPY_AND_ASSIGN(Prefix);
        };


        /// 指定した出力先(Output)の最初にPrefixをつけて、append()で指定したbufferを出力する。
        class StringBuffer final
        {
        public:
            StringBuffer(std::unique_ptr<Output>&& output, std::unique_ptr<Prefix>&& prefix);
            ~StringBuffer() = default;

            void append(const Buffer& buffer);
            void flush();

        private:
            void appendPrefix(Buffer& buffer);

            std::unique_ptr<Output> output_;
            std::unique_ptr<Prefix> prefix_;
            static thread_local Buffer buffer_thl_;

        private:
            DISALLOW_COPY_AND_ASSIGN(StringBuffer);
        };

    }
}

#endif // BKZ_LOG_STRINGBUFFER_H
