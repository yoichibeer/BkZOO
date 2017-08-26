/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZOO_TUT_H
#define BKZOO_TUT_H

#include <string>

namespace tut
{

    // ensure関数のmessage引数に__LINE__を文字列として埋め込むために使用する
    std::string M(int lineNum);

}

#endif // BKZOO_TUT_H
