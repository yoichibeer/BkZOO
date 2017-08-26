/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Registry.h"

#include "bkzoo_string.h"

#include "bkzoo_log.h"

#include <sstream>

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace registry
    {

        typedef void (WINAPI * LPFN_GETNATIVESYSTEMINFO)(LPSYSTEM_INFO);

        BOOL is_x64(void)
        {
            LPFN_GETNATIVESYSTEMINFO fnGetNativeSystemInfo =
                reinterpret_cast<LPFN_GETNATIVESYSTEMINFO>(GetProcAddress(GetModuleHandle(L"kernel32"), "GetNativeSystemInfo"));
            if (fnGetNativeSystemInfo == nullptr)
            {
                return FALSE;
            }

            BOOL bIsx64 = FALSE;
            SYSTEM_INFO info;
            fnGetNativeSystemInfo(&info);
            if (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
            {
                bIsx64 = TRUE;
            }
            else if (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
            {
                bIsx64 = TRUE;
            }
            else if (info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
            {
                bIsx64 = FALSE;
            }
            else
            {
                // do nothing
            }

            return bIsx64;
        }


        Registry::Registry(HKEY hParentKey, const std::wstring& subKey)
            : hKey_(nullptr), hParentKey_(hParentKey), subKey_(subKey)
        {
        }

        Registry::~Registry()
        {
            if (hKey_ == nullptr)
            {
                return;
            }

            LSTATUS lstatus = ::RegCloseKey(hKey_);
            if (lstatus != ERROR_SUCCESS)
            {
                LOG_ERROR << lstatus;
            }
        }

        HKEY Registry::regCreateKey()
        {
            DWORD dwDisposition = 0;
            LSTATUS lStatus = ::RegCreateKeyEx(hParentKey_, subKey_.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_CREATE_SUB_KEY | KEY_SET_VALUE | KEY_WOW64_64KEY, nullptr, &hKey_, &dwDisposition);
            if (lStatus != ERROR_SUCCESS)
            {
                hKey_ = nullptr;
                return hKey_;
            }

            LOG_INFO << ((dwDisposition == REG_CREATED_NEW_KEY) ? "REG_CREATED_NEW_KEY" : "REG_OPENED_EXISTING_KEY");

            return hKey_;
        }
        HKEY Registry::regOpenKey(REGSAM regsam)
        {
            LSTATUS lStatus = ::RegOpenKeyEx(hParentKey_, subKey_.c_str(), 0, regsam, &hKey_);
            if (lStatus != ERROR_SUCCESS)
            {
                hKey_ = nullptr;
                return hKey_;
            }
            return hKey_;
        }
        HKEY Registry::regOpenKey_READ()
        {
            return regOpenKey(KEY_READ | KEY_WOW64_64KEY);
        }
        HKEY Registry::regOpenKey_READ_WRITE()
        {
            return regOpenKey(KEY_READ | KEY_WRITE | KEY_WOW64_64KEY);
        }

        bool Registry::regQueryValue(const wchar_t* valueName, std::wstring& value) const
        {
            assert(valueName != nullptr);

            value.clear();

            BYTE buffer[_MAX_PATH] = { 0 };
            DWORD cBuf = sizeof(buffer);
            LSTATUS lstatus = ::RegQueryValueEx(hKey_, valueName, nullptr, nullptr, buffer, &cBuf);
            if (lstatus != ERROR_SUCCESS)
            {
                LOG_WARNING << ": valueName=" << valueName << ": buffer=" << buffer << ": lstatus=" << lstatus;
                return false;
            }

            value = reinterpret_cast<wchar_t*>(buffer);

            return true;
        }

        void Registry::regDeleteValue(const wchar_t* valueName) const
        {
            if (hKey_ == nullptr)
            {
                return;
            }

            LONG result = ::RegDeleteValue(hKey_, valueName);
            if (result != ERROR_SUCCESS)
            {
                LOG_WARNING << ": valueName=" << valueName << ": result=" << result;
            }
        }

        void Registry::regSetValue(const wchar_t* valueName, const std::wstring& data)
        {
            if (hKey_ == nullptr)
            {
                return;
            }

            LONG result = ::RegSetValueEx(hKey_, valueName, 0, REG_SZ, reinterpret_cast<const BYTE*>(data.c_str()), data.size() * sizeof(wchar_t));
            if (result != ERROR_SUCCESS)
            {
                LOG_ERROR << ": result=" << result;
            }
        }

        // 指定したアプリのインストールパスを取得
        // param [in] applicationName インストールパスを取得したいアプリ名
        // param [out] installLocation 取得したインストールパスをここに格納
        // return 存在した場合はtrueを返却
        bool Registry::getInstallLocation(const wchar_t* applicationName, std::wstring* pInstallLocation)
        {
            std::wstring installLocation;

            if (applicationName == nullptr)
            {
                return false;
            }

            std::wostringstream registryPath;
            registryPath << L"SOFTWARE\\";
            if (is_x64())
            {
                registryPath << L"Wow6432Node\\";
            }
            registryPath << L"Microsoft\\Windows\\CurrentVersion\\Uninstall\\" << applicationName;

            Registry registry(HKEY_LOCAL_MACHINE, registryPath.str().c_str());
            HKEY hKey = registry.regOpenKey_READ();
            if (hKey == nullptr)
            {
                return false;
            }

            bool success = registry.regQueryValue(L"InstallLocation", installLocation);
            if (!success)
            {
                return false;
            }

            if (installLocation.empty())
            {
                return false;
            }

            if (pInstallLocation != nullptr)
            {
                *pInstallLocation = installLocation;
            }

            return true;
        }


        // TortoiseSVNのインストールパスを
        // param [out] installLocation 取得したインストールパスをここに格納
        // return 存在した場合はtrueを返却
        bool Registry::getTortoiseProcInstallLocation(std::wstring* pInstallLocation)
        {
            std::wstring installLocation;

            Registry registry(HKEY_LOCAL_MACHINE, L"SOFTWARE\\TortoiseSVN");
            HKEY hKey = registry.regOpenKey_READ();
            if (hKey == nullptr)
            {
                return false;
            }

            bool success = registry.regQueryValue(L"ProcPath", installLocation);
            if (!success)
            {
                return false;
            }

            if (installLocation.empty())
            {
                return false;
            }

            if (pInstallLocation != nullptr)
            {
                *pInstallLocation = installLocation;
            }

            return true;
        }

        bool Registry::isSkipWarningOpenUrl()
        {
            std::wostringstream registryPath;
            registryPath << REG_PATH_DontShowMeThisDialogAgain;

            Registry registry(HKEY_CURRENT_USER, registryPath.str().c_str());
            HKEY hKey = registry.regOpenKey_READ();
            if (hKey == nullptr)
            {
                return false;
            }

            std::wstring dontShowMeThisDialogAgainValue;
            bool success = registry.regQueryValue(Registry::REG_VALUE_GUID, dontShowMeThisDialogAgainValue);
            if (!success)
            {
                return false;
            }

            if (dontShowMeThisDialogAgainValue.empty())
            {
                return false;
            }

            return dontShowMeThisDialogAgainValue == L"NO";
        }

        void Registry::removeIgnoreFlagForWarningOpenUrl()
        {
            std::wostringstream registryPath;
            registryPath << REG_PATH_DontShowMeThisDialogAgain;

            Registry registry(HKEY_CURRENT_USER, registryPath.str().c_str());
            HKEY hKey = registry.regOpenKey_READ_WRITE();
            if (hKey == nullptr)
            {
                return;
            }

            registry.regDeleteValue(Registry::REG_VALUE_GUID);
        }

        void Registry::addIgnoreFlagForWarningOpenUrl()
        {
            std::wostringstream registryPath;
            registryPath << REG_PATH_DontShowMeThisDialogAgain;

            Registry registry(HKEY_CURRENT_USER, registryPath.str().c_str());
            HKEY hKey = registry.regCreateKey();
            if (hKey == nullptr)
            {
                return;
            }

            registry.regSetValue(Registry::REG_VALUE_GUID, L"NO");
        }

    }
}
