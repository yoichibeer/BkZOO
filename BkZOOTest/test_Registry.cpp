/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include "registry/Registry.h"

#include "debug/detect_memory_leak.h"

using namespace bkzoo::registry;

namespace tut
{

    // Data used by each test
    struct Registry_data
    {
        virtual ~Registry_data()
        {
        }
    };

    // Test group registration
    typedef test_group<Registry_data> factory;
    typedef factory::object object;

}

namespace
{

    tut::factory tf("Registry");

}

namespace tut
{

    // regOpenKey_READ, regQueryValue
    template<>
    template<>
    void object::test<1>()
    {
        Registry registrySYSTEM(HKEY_LOCAL_MACHINE, L"SYSTEM");
        HKEY hKey = registrySYSTEM.regOpenKey_READ();
        ensure(M(__LINE__), hKey != nullptr);

        Registry registryCurrentControlSet(hKey, L"CurrentControlSet");
        hKey = registryCurrentControlSet.regOpenKey_READ();
        ensure(M(__LINE__), hKey != nullptr);

        Registry registry_services(hKey, L"services");
        hKey = registry_services.regOpenKey_READ();
        ensure(M(__LINE__), hKey != nullptr);

        Registry registry_i8042prt(hKey, L"i8042prt");
        hKey = registry_i8042prt.regOpenKey_READ();
        ensure(M(__LINE__), hKey != nullptr);

        Registry registryParameters(hKey, L"Parameters");
        hKey = registryParameters.regOpenKey_READ();
        ensure(M(__LINE__), hKey != nullptr);

        std::wstring queryValue;
        bool success = registryParameters.regQueryValue(L"LayerDriver JPN", queryValue);
        ensure(M(__LINE__), success);

        ensure_equals(M(__LINE__), queryValue, L"kbd106.dll");
    }


    // regOpenKey_READ, regQueryValue
    template<>
    template<>
    void object::test<2>()
    {
        if (is_x64())
        {
            Registry registrySYSTEM(HKEY_LOCAL_MACHINE, L"SOFTWARE");
            HKEY hKey = registrySYSTEM.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryWow6432Node(hKey, L"Wow6432Node");
            hKey = registryWow6432Node.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryMicrosoft(hKey, L"Microsoft");
            hKey = registryMicrosoft.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryWindows(hKey, L"Windows");
            hKey = registryWindows.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryCurrentVersion(hKey, L"CurrentVersion");
            hKey = registryCurrentVersion.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryUninstall(hKey, L"Uninstall");
            hKey = registryUninstall.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryApplicationName(hKey, L"FFFTP");
            hKey = registryApplicationName.regOpenKey_READ();
            if (hKey != nullptr)
            {
                std::wstring queryValue;
                bool success = registryApplicationName.regQueryValue(L"InstallLocation", queryValue);
                ensure(M(__LINE__), success);
            }
        }
        else
        {
            Registry registrySYSTEM(HKEY_LOCAL_MACHINE, L"SOFTWARE");
            HKEY hKey = registrySYSTEM.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryMicrosoft(hKey, L"Microsoft");
            hKey = registryMicrosoft.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryWindows(hKey, L"Windows");
            hKey = registryWindows.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryCurrentVersion(hKey, L"CurrentVersion");
            hKey = registryCurrentVersion.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryUninstall(hKey, L"Uninstall");
            hKey = registryUninstall.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            Registry registryApplicationName(hKey, L"FFFTP");
            hKey = registryApplicationName.regOpenKey_READ();
            ensure(M(__LINE__), hKey != nullptr);

            std::wstring queryValue;
            bool success = registryApplicationName.regQueryValue(L"InstallLocation", queryValue);
            ensure(M(__LINE__), success);
        }
    }


    // getInstallLocation
    template<>
    template<>
    void object::test<3>()
    {
        if (is_x64())
        {
            std::wstring installLocation;
            bool ret = Registry::getInstallLocation(nullptr, &installLocation);
            ensure_equals(M(__LINE__), ret, false);

            ret = Registry::getInstallLocation(nullptr, nullptr);
            ensure_equals(M(__LINE__), ret, false);

            ret = Registry::getInstallLocation(L"FFFTP", &installLocation);
            if (ret)
            {
                ensure_equals(M(__LINE__), installLocation, L"C:\\Program Files (x86)\\ffftp\\");
            }
            else
            {
                skip(M(__LINE__) + " not exists FFFTP");
            }
        }
        else
        {
            std::wstring installLocation;

            bool ret = Registry::getInstallLocation(nullptr, &installLocation);
            ensure_equals(M(__LINE__), ret, false);

            ret = Registry::getInstallLocation(nullptr, nullptr);
            ensure_equals(M(__LINE__), ret, false);

            ret = Registry::getInstallLocation(L"FFFTP", &installLocation);
            if (ret)
            {
                ensure_equals(M(__LINE__), installLocation, L"C:\\Program Files\\ffftp\\");
            }
            else
            {
                skip(M(__LINE__) + " not exists FFFTP");
            }
        }
    }


    // getTortoiseProcInstallLocation
    template<>
    template<>
    void object::test<4>()
    {
        std::wstring tortoiseProcInstallLocation;
        bool ret = Registry::getTortoiseProcInstallLocation(&tortoiseProcInstallLocation);
        bool ret_nullptr = Registry::getTortoiseProcInstallLocation(nullptr);

        if (ret)
        {
            ensure_equals(M(__LINE__), ret_nullptr, true);
            ensure_equals(M(__LINE__),
                tortoiseProcInstallLocation,
                L"C:\\Program Files\\TortoiseSVN\\bin\\TortoiseProc.exe");

        }
        else
        {
            ensure_equals(M(__LINE__), ret_nullptr, false);
            skip(M(__LINE__) + " not exists TortoiseSVN");
        }
    }


    // Error
    template<>
    template<>
    void object::test<5>()
    {
        Registry registry(HKEY_LOCAL_MACHINE, L"NO_EXISTS_SUB_KEY");
        HKEY hkey = registry.regOpenKey_READ();
        ensure_equals(M(__LINE__), hkey, static_cast<HKEY>(nullptr));

        std::wstring queryValue(L"dummy");
        bool ret = registry.regQueryValue(L"", queryValue);
        ensure_equals(M(__LINE__), ret, false);
        ensure_equals(M(__LINE__), queryValue, L"");
    }


    // is_x64
    // 環境依存なので動作確認のみでここでテスト判定はしていない
    template<>
    template<>
    void object::test<6>()
    {
        BOOL x64 = is_x64();
        if (x64)
        {
            printf("CPU architecture is x64");
        }
        else
        {
            printf("CPU architecture is x86");
        }
    }


    // warningOpenUrl
    template<>
    template<>
    void object::test<7>()
    {
        bool ret = Registry::isSkipWarningOpenUrl();

        if (ret)
        {
            Registry::removeIgnoreFlagForWarningOpenUrl();
            ensure_equals(Registry::isSkipWarningOpenUrl(), false);

            Registry::addIgnoreFlagForWarningOpenUrl();
            ensure_equals(Registry::isSkipWarningOpenUrl(), true);
        }
        else
        {
            Registry::addIgnoreFlagForWarningOpenUrl();
            ensure_equals(Registry::isSkipWarningOpenUrl(), true);

            Registry::removeIgnoreFlagForWarningOpenUrl();
            ensure_equals(Registry::isSkipWarningOpenUrl(), false);
        }
    }

}
