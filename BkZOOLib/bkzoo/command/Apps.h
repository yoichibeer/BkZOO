/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_COMMAND_APPS_H
#define BKZ_COMMAND_APPS_H

#include "Command.h"

#include "url/URL.h"

#include <Windows.h>

#include <string>

namespace bkzoo
{
    namespace command
    {

        typedef struct AppsParam
        {
            HWND hWnd;
            std::wstring selectedString;
            std::wstring installLocationKey;
            std::wstring installLocationFileName;
            unsigned long availableSchemes;
            url::Scheme primaryScheme;

            static AppsParam createParamOfFFFTP(HWND hWnd, const std::wstring& selectedString);
            static AppsParam createParamOfFileZilla(HWND hWnd, const std::wstring& selectedString);
            static AppsParam createParamOfWinSCP(HWND hWnd, const std::wstring& selectedString);
            static AppsParam createParamOfTeraTerm(HWND hWnd, const std::wstring& selectedString);

        } AppsParam;


        // 各種アプリケーションを呼び出すために共通して使用するクラス
        class AppsCommand final : public Command
        {
        public:
            explicit AppsCommand(const AppsParam& param);
            virtual bool execute() override;

        private:
            const AppsParam param_;
        };

    }
}

#endif // BKZ_COMMAND_APPS_H
