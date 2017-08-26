/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_LOG_H
#define BKZ_LOG_H

#include "log/Logger.h"

using namespace bkzoo::log;

// ログレベルを最後に出力する場合はこちら。とりあえずコメントアウト。
//#define BKZ_LOG_COMMON(BKZ_LOG_LEVEL) __FILE__ << "(" << __LINE__ << ") <" << __FUNCTION__ << "> [" << #BKZ_LOG_LEVEL << "]:　"
//#define BKZ_LOG(BKZ_LOG_LEVEL) \
//    for (int bkzoo_log_index = 0; bkzoo_log_index < 1; ++bkzoo_log_index, ::bkzoo::log::Logger::instance() << ::bkzoo::log::endl) \
//        ::bkzoo::log::Logger::instance() << ::bkzoo::log::LogLevel::BKZ_LOG_LEVEL << BKZ_LOG_COMMON(BKZ_LOG_LEVEL)

#define BKZ_LOG_COMMON __FILE__ << "(" << __LINE__ << ") <" << __FUNCTION__ << ">: "
#define BKZ_LOG(BKZ_LOG_LEVEL) \
    for (int bkzoo_log_index = 0; bkzoo_log_index < 1; ++bkzoo_log_index, ::bkzoo::log::Logger::instance() << ::bkzoo::log::bkzEndl) \
        ::bkzoo::log::Logger::instance() << ::bkzoo::log::LogLevel::BKZ_LOG_LEVEL << BKZ_LOG_COMMON

#define LOG_TRACE BKZ_LOG(Trace)
#define LOG_DEBUG BKZ_LOG(Debug)
#define LOG_INFO BKZ_LOG(Info)
#define LOG_WARNING BKZ_LOG(Warning)
#define LOG_ERROR BKZ_LOG(Error)
#define LOG_FATAL BKZ_LOG(Fatal)

#endif // BKZ_LOG_H
