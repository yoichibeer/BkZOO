/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_SEARCH_WORD_H
#define BKZ_COMMAND_SEARCH_WORD_H

#include "Command.h"
#include "url/URLEncode.h"
#include "bkzoo_config.h"

#include <Windows.h>

namespace bkzoo
{
    namespace command
    {

        // 文字列を検索で実行されるクラス.
        class SearchWordCommand final : public Command
        {
        public:
            typedef struct Param
            {
                HWND hWnd;
                std::wstring selectedString;
                std::wstring regexpString;
                std::wstring url;
                int encode;
                MethodType method;
                url::fn_urlEncode urlEncode;
            } Param;

        public:
            explicit SearchWordCommand(const Param& param);
            virtual bool execute() override;

        private:
            static std::string generatePostHtmlFile(const std::wstring& searchUrl, int encode);

        private:
            const Param param_;
        };

    }
}

#endif // BKZ_COMMAND_SEARCH_WORD_H
