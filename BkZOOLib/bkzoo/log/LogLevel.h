/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_LOG_LOGLEVEL_H
#define BKZ_LOG_LOGLEVEL_H

namespace bkzoo
{
    namespace log
    {
        /**
         * よくあるログの仕様と同じ。あるログレベルよりも上のレベルを指定すると出力しないで、あるログレベル以下を指定すると出力する。
         * Loggerクラスの setMaximumOutputLogLevel() で最大出力ログレベルを設定後、
         * 各ログ出力のレベルを Logger::operator<<(LogLevel) で指定して、この指定が、事前に設定した最大出力ログレベル以下なら出力する。
         */
        enum class LogLevel : int
        {
            Non = 0,
            Fatal,
            Error,
            Warning,
            Info,
            Debug,
            Trace,
        };

    }
}

#endif // BKZ_LOG_LOGLEVEL_H
