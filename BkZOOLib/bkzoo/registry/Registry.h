/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_REGISTRY_REGISTRY_H
#define BKZ_REGISTRY_REGISTRY_H

#include <Windows.h>

#include <string>

#include "defs.h"

namespace bkzoo
{
    namespace registry
    {

        // x64か否か
        BOOL is_x64();

        class Registry final
        {
        public:
            Registry(HKEY hParentKey, const std::wstring& subKey);
            ~Registry();

            HKEY regCreateKey();

            HKEY regOpenKey_READ();
            HKEY regOpenKey_READ_WRITE();


            // regOpenKey_READで取得したキーにあるValueを取得する
            // param [out] value valueNameをKeyとしてこの引数にValueを格納する。
            // エラー時は空文字を格納する（valueの初期値に関わらず）。
            // return エラー時はfalseを返す。
            bool regQueryValue(const wchar_t* valueName, std::wstring& value) const;

            // regOpenKey_READで取得したキーにあるValueを削除する
            void regDeleteValue(const wchar_t* valueName) const;

            void regSetValue(const wchar_t* valueName, const std::wstring& data);

            static bool getInstallLocation(const wchar_t* applicationName, std::wstring* pInstallLocation);
            static bool getTortoiseProcInstallLocation(std::wstring* pInstallLocation);
            static bool getIexploreInstallLocation(std::wstring* pInstallLocation);

            static bool isSkipWarningOpenUrl();
            static void removeIgnoreFlagForWarningOpenUrl();
            static void addIgnoreFlagForWarningOpenUrl();
            static bool isDefaultBrowzerIExplorer();

            static constexpr wchar_t * const REG_VALUE_GUID = L"{36EEF0A3-222E-446F-9DA9-8DF07F7BFC83}";

        private:
            HKEY hKey_;
            HKEY hParentKey_;
            std::wstring subKey_;

            HKEY regOpenKey(REGSAM regsam);

            static constexpr wchar_t * const REG_PATH_DontShowMeThisDialogAgain = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\DontShowMeThisDialogAgain";

        private:
            DISALLOW_COPY_AND_ASSIGN(Registry);
        };

    }
}


#endif // BKZ_REGISTRY_REGISTRY_H
