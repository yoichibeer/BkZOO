/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#include <gtest/gtest.h>

#include "registry\Registry.h"

#include "debug/detect_memory_leak.h"

using namespace bkzoo::registry;

// regOpenKey_READ, regQueryValue
TEST(RegistryTest, regOpenKey_READ)
{
    Registry registrySYSTEM(HKEY_LOCAL_MACHINE, L"SYSTEM");
    HKEY hKey = registrySYSTEM.regOpenKey_READ();
    EXPECT_NE(nullptr, hKey);

    Registry registryCurrentControlSet(hKey, L"CurrentControlSet");
    hKey = registryCurrentControlSet.regOpenKey_READ();
    EXPECT_NE(nullptr, hKey);

    Registry registry_services(hKey, L"services");
    hKey = registry_services.regOpenKey_READ();
    EXPECT_NE(nullptr, hKey);

    Registry registry_i8042prt(hKey, L"i8042prt");
    hKey = registry_i8042prt.regOpenKey_READ();
    EXPECT_NE(nullptr, hKey);

    Registry registryParameters(hKey, L"Parameters");
    hKey = registryParameters.regOpenKey_READ();
    EXPECT_NE(nullptr, hKey);

    std::wstring queryValue;
    const bool success = registryParameters.regQueryValue(L"LayerDriver JPN", queryValue);
    EXPECT_TRUE(success);

    EXPECT_EQ(L"kbd106.dll", queryValue);
}

// regOpenKey_READ, regQueryValue
TEST(RegistryTest, regOpenKey_READ_FFFTP)
{
    if (is_x64())
    {
        Registry registrySYSTEM(HKEY_LOCAL_MACHINE, L"SOFTWARE");
        HKEY hKey = registrySYSTEM.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryWow6432Node(hKey, L"Wow6432Node");
        hKey = registryWow6432Node.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryMicrosoft(hKey, L"Microsoft");
        hKey = registryMicrosoft.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryWindows(hKey, L"Windows");
        hKey = registryWindows.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryCurrentVersion(hKey, L"CurrentVersion");
        hKey = registryCurrentVersion.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryUninstall(hKey, L"Uninstall");
        hKey = registryUninstall.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryApplicationName(hKey, L"FFFTP");
        hKey = registryApplicationName.regOpenKey_READ();
        if (hKey != nullptr)
        {
            std::wstring queryValue;
            const bool success = registryApplicationName.regQueryValue(L"InstallLocation", queryValue);
            EXPECT_TRUE(success);
        }
    }
    else
    {
        Registry registrySYSTEM(HKEY_LOCAL_MACHINE, L"SOFTWARE");
        HKEY hKey = registrySYSTEM.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryMicrosoft(hKey, L"Microsoft");
        hKey = registryMicrosoft.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryWindows(hKey, L"Windows");
        hKey = registryWindows.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryCurrentVersion(hKey, L"CurrentVersion");
        hKey = registryCurrentVersion.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryUninstall(hKey, L"Uninstall");
        hKey = registryUninstall.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        Registry registryApplicationName(hKey, L"FFFTP");
        hKey = registryApplicationName.regOpenKey_READ();
        EXPECT_NE(nullptr, hKey);

        std::wstring queryValue;
        const bool success = registryApplicationName.regQueryValue(L"InstallLocation", queryValue);
        EXPECT_TRUE(success);
    }
}


// getInstallLocation
TEST(RegistryTest, getInstallLocation)
{
    if (is_x64())
    {
        std::wstring installLocation;
        bool ret = Registry::getInstallLocation(nullptr, &installLocation);
        EXPECT_FALSE(ret);

        ret = Registry::getInstallLocation(nullptr, nullptr);
        EXPECT_FALSE(ret);

        ret = Registry::getInstallLocation(L"FFFTP", &installLocation);
        if (ret)
        {
            EXPECT_EQ(L"C:\\Program Files (x86)\\ffftp\\", installLocation);
        }
        else
        {
            std::cout << " not exists FFFTP";
            return;
        }
    }
    else
    {
        std::wstring installLocation;

        bool ret = Registry::getInstallLocation(nullptr, &installLocation);
        EXPECT_FALSE(ret);

        ret = Registry::getInstallLocation(nullptr, nullptr);
        EXPECT_FALSE(ret);

        ret = Registry::getInstallLocation(L"FFFTP", &installLocation);
        if (ret)
        {
            EXPECT_EQ(L"C:\\Program Files\\ffftp\\", installLocation);
        }
        else
        {
            std::cout << " not exists FFFTP";
            return;
        }
    }
}

// getTortoiseProcInstallLocation
TEST(RegistryTest, getTortoiseProcInstallLocation)
{
    std::wstring tortoiseProcInstallLocation;
    bool ret = Registry::getTortoiseProcInstallLocation(&tortoiseProcInstallLocation);
    bool ret_nullptr = Registry::getTortoiseProcInstallLocation(nullptr);

    if (ret)
    {
        EXPECT_TRUE(ret_nullptr);
        EXPECT_EQ(L"C:\\Program Files\\TortoiseSVN\\bin\\TortoiseProc.exe"
            , tortoiseProcInstallLocation);
    }
    else
    {
        EXPECT_FALSE(ret_nullptr);
        std::cout << " not exists TortoiseSVN";
        return;
    }
}

// getIexploreInstallLocation
TEST(RegistryTest, getIexploreInstallLocation)
{
    std::wstring iexploreInstallLocation;
    bool ret = Registry::getIexploreInstallLocation(&iexploreInstallLocation);
    bool ret_nullptr = Registry::getIexploreInstallLocation(nullptr);

    if (ret)
    {
        EXPECT_TRUE(ret_nullptr);
        EXPECT_EQ(L"C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE"
            , iexploreInstallLocation);
    }
    else
    {
        EXPECT_FALSE(ret_nullptr);
        std::cout << " not exists iexplore.exe";
        return;
    }
}


// Error
TEST(RegistryTest, Error)
{
    Registry registry(HKEY_LOCAL_MACHINE, L"NO_EXISTS_SUB_KEY");
    HKEY hkey = registry.regOpenKey_READ();
    EXPECT_EQ(nullptr, hkey);

    std::wstring queryValue(L"dummy");
    bool ret = registry.regQueryValue(L"", queryValue);
    EXPECT_FALSE(ret);
    EXPECT_EQ(L"", queryValue);
}


// is_x64
// 環境依存なので動作確認のみでここでテスト判定はしていない
TEST(RegistryTest, is_x64)
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
TEST(RegistryTest, warningOpenUrl)
{
    bool ret = Registry::isSkipWarningOpenUrl();

    if (ret)
    {
        Registry::removeIgnoreFlagForWarningOpenUrl();
        EXPECT_FALSE(Registry::isSkipWarningOpenUrl());

        Registry::addIgnoreFlagForWarningOpenUrl();
        EXPECT_TRUE(Registry::isSkipWarningOpenUrl());
    }
    else
    {
        Registry::addIgnoreFlagForWarningOpenUrl();
        EXPECT_TRUE(Registry::isSkipWarningOpenUrl());

        Registry::removeIgnoreFlagForWarningOpenUrl();
        EXPECT_FALSE(Registry::isSkipWarningOpenUrl());
    }
}


// isDefaultBrowzerIExplorer
TEST(RegistryTest, isDefaultBrowzerIExplorer)
{
    // bool ret = 
    Registry::isDefaultBrowzerIExplorer();
    // 以下、開発環境がChromeデフォルトなのでコメントアウト。一度だけIEデフォルトにして確認済み。
    // EXPECT_TRUE(ret);

    // 必ずfalseになる。ret変数未使用のwarning回避目的。
    // EXPECT_FALSE(ret && !ret);
}
