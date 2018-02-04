#include <gtest/gtest.h>

#include "config/ConfigRepository.h"

#include "for_debug/detect_memory_leak.h"

using namespace bkzoo::config;


TEST(ConfigRepository, defaultValue)
{
    ConfigRepository configRepository;
    const bool ret = configRepository.initialize(L"", L""); // iniファイルを指定していない。デフォルト値で初期化されるはず。
    ASSERT_TRUE(ret);

    const General general = configRepository.readGeneral();
    const General generalDefault;

    EXPECT_EQ(generalDefault.tripleClick(), general.tripleClick());
    EXPECT_EQ(generalDefault.ctrlDoubleClick(), general.ctrlDoubleClick());
    EXPECT_EQ(generalDefault.ctrlTripleClick(), general.ctrlTripleClick());
    EXPECT_EQ(generalDefault.version(), general.version());
    EXPECT_EQ(generalDefault.checkBeckyUpdate(), general.checkBeckyUpdate());
    EXPECT_EQ(generalDefault.warningOpenUrl(), general.warningOpenUrl());
    EXPECT_EQ(generalDefault.logLevel(), general.logLevel());

    EXPECT_EQ(generalDefault.shortcut(App::Browzer), general.shortcut(App::Browzer));
    EXPECT_EQ(generalDefault.shortcut(App::Explorer), general.shortcut(App::Explorer));
    EXPECT_EQ(generalDefault.shortcut(App::FFFTP), general.shortcut(App::FFFTP));
    EXPECT_EQ(generalDefault.shortcut(App::FileZilla), general.shortcut(App::FileZilla));
    EXPECT_EQ(generalDefault.shortcut(App::Google), general.shortcut(App::Google));
    EXPECT_EQ(generalDefault.shortcut(App::PasteBracket), general.shortcut(App::PasteBracket));
    EXPECT_EQ(generalDefault.shortcut(App::RemoteDesktop), general.shortcut(App::RemoteDesktop));
    EXPECT_EQ(generalDefault.shortcut(App::TeraTerm), general.shortcut(App::TeraTerm));
    EXPECT_EQ(generalDefault.shortcut(App::TortoiseSVN), general.shortcut(App::TortoiseSVN));
    EXPECT_EQ(generalDefault.shortcut(App::WinSCP), general.shortcut(App::WinSCP));

    const SiteMap presetSites = configRepository.readPresetSites();
    EXPECT_EQ(0U, presetSites.size());

    const SiteVector standardSites = configRepository.readStandardSites();
    EXPECT_EQ(0U, standardSites.size());
}


TEST(ConfigRepository, readGeneral)
{
    ConfigRepository configRepository;
    const bool ret = configRepository.initialize(L"../UnitTest/testdata/BkZOO.ini", L"");
    ASSERT_TRUE(ret);

    const General general = configRepository.readGeneral();

    EXPECT_EQ(L"true", general.tripleClick());
    EXPECT_TRUE(general.tripleClick_bool());

    EXPECT_EQ(L"true", general.ctrlDoubleClick());
    EXPECT_TRUE(general.ctrlDoubleClick_bool());

    EXPECT_EQ(L"false", general.ctrlTripleClick());
    EXPECT_FALSE(general.ctrlTripleClick_bool());

    EXPECT_EQ(L"2.72.01", general.version());
    EXPECT_EQ(L"true", general.checkBeckyUpdate());
    EXPECT_EQ(L"false", general.warningOpenUrl());
    EXPECT_EQ(L"2", general.logLevel());

    EXPECT_EQ(L'1', general.shortcut(App::Browzer));
    EXPECT_EQ(L'3', general.shortcut(App::Explorer));
    EXPECT_EQ(L'T', general.shortcut(App::FFFTP));
    EXPECT_EQ(L'Z', general.shortcut(App::FileZilla));
    EXPECT_EQ(L'G', general.shortcut(App::Google));
    EXPECT_EQ(L'K', general.shortcut(App::PasteBracket));
    EXPECT_EQ(L'R', general.shortcut(App::RemoteDesktop));
    EXPECT_EQ(L'M', general.shortcut(App::TeraTerm));
    EXPECT_EQ(L'V', general.shortcut(App::TortoiseSVN));
    EXPECT_EQ(L'W', general.shortcut(App::WinSCP));
}


TEST(ConfigRepository, readPresetSites)
{
    ConfigRepository configRepository;
    const bool ret = configRepository.initialize(L"", L"../UnitTest/testdata/BkZOO.preset.ini");
    ASSERT_TRUE(ret);

    const SiteMap sites = configRepository.readPresetSites();
    ASSERT_EQ(21, sites.size());

    {
        const Site& site = sites.at(6);
        EXPECT_EQ(L"6", site.presetid());
        EXPECT_EQ(6, site.presetid_int());
        EXPECT_EQ(L"true", site.preset());
        EXPECT_TRUE(site.preset_bool());
        EXPECT_EQ(L"false", site.enabled());
        EXPECT_FALSE(site.enabled_bool());
        EXPECT_EQ(L"Excite翻訳", site.title());
        EXPECT_EQ(L"D", site.shortcut());
        EXPECT_EQ(L'D', site.shortcut_char());
        EXPECT_EQ(L"http://www.excite.co.jp/world/english/?auto_detect=on&before={}", site.url());
        EXPECT_EQ(L"UTF-8", site.encode());
        EXPECT_EQ(CP_UTF8, site.encode_int());
        EXPECT_EQ(L"POST", site.method());
        EXPECT_EQ(MethodType::POST, site.method_enum());
        EXPECT_EQ(L"", site.regex());
        EXPECT_EQ(L"false", site.clickable());
        EXPECT_FALSE(site.clickable_bool());
    }
    {
        const Site& site = sites.at(20);
        EXPECT_EQ(L"20", site.presetid());
        EXPECT_EQ(20, site.presetid_int());
        EXPECT_EQ(L"true", site.preset());
        EXPECT_TRUE(site.preset_bool());
        EXPECT_EQ(L"false", site.enabled());
        EXPECT_FALSE(site.enabled_bool());
        EXPECT_EQ(L"Redmineチケット番号で検索", site.title());
        EXPECT_EQ(L"9", site.shortcut());
        EXPECT_EQ(L'9', site.shortcut_char());
        EXPECT_EQ(L"http://www.redmine.org/issues/{}", site.url());
        EXPECT_EQ(L"UTF-8", site.encode());
        EXPECT_EQ(CP_UTF8, site.encode_int());
        EXPECT_EQ(L"GET", site.method());
        EXPECT_EQ(MethodType::GET, site.method_enum());
        EXPECT_EQ(L"^#?(\\d+)$", site.regex());
        EXPECT_EQ(L"true", site.clickable());
        EXPECT_TRUE(site.clickable_bool());
    }
}


TEST(ConfigRepository, readStandardSites)
{
    ConfigRepository configRepository;
    const bool ret = configRepository.initialize(L"../UnitTest/testdata/BkZOO.ini", L"");
    ASSERT_TRUE(ret);

    const SiteVector standardSites = configRepository.readStandardSites();
    ASSERT_EQ(3, standardSites.size());

    {
        const Site& site = standardSites.at(0);
        EXPECT_EQ(L"false", site.preset());
        EXPECT_FALSE(site.preset_bool());
        EXPECT_EQ(L"true", site.enabled());
        EXPECT_TRUE(site.enabled_bool());
        EXPECT_EQ(L"Google検索2", site.title());
        EXPECT_EQ(L"8", site.shortcut());
        EXPECT_EQ(L'8', site.shortcut_char());
        EXPECT_EQ(L"http://www.google.co.jp/search?hl=ja&lr=lang_ja&ie=UTF-8&q={}", site.url());
        EXPECT_EQ(L"UTF-8", site.encode());
        EXPECT_EQ(CP_UTF8, site.encode_int());
        EXPECT_EQ(L"GET", site.method());
        EXPECT_EQ(MethodType::GET, site.method_enum());
        EXPECT_EQ(L"", site.regex());
        EXPECT_EQ(L"true", site.clickable());
        EXPECT_TRUE(site.clickable_bool());
    }
    {
        const Site& site = standardSites.at(1);
        EXPECT_EQ(L"true", site.preset());
        EXPECT_TRUE(site.preset_bool());
        EXPECT_EQ(L"false", site.enabled());
        EXPECT_FALSE(site.enabled_bool());
        EXPECT_EQ(L"", site.title());
        EXPECT_EQ(L"", site.shortcut());
        EXPECT_EQ(L'\0', site.shortcut_char());
        EXPECT_EQ(L"", site.url());
        EXPECT_EQ(L"", site.encode());
        EXPECT_EQ(CP_ACP, site.encode_int());
        EXPECT_EQ(L"", site.method());
        EXPECT_EQ(MethodType::GET, site.method_enum());
        EXPECT_EQ(L"", site.regex());
        EXPECT_EQ(L"", site.clickable());
        EXPECT_FALSE(site.clickable_bool());
    }
    {
        const Site& site = standardSites.at(2);
        EXPECT_EQ(L"false", site.preset());
        EXPECT_FALSE(site.preset_bool());
        EXPECT_EQ(L"false", site.enabled());
        EXPECT_FALSE(site.enabled_bool());
        EXPECT_EQ(L"Yahoo!で検索2", site.title());
        EXPECT_EQ(L"Y", site.shortcut());
        EXPECT_EQ(L'Y', site.shortcut_char());
        EXPECT_EQ(L"http://search.yahoo.co.jp/search?p={}", site.url());
        EXPECT_EQ(L"UTF-8", site.encode());
        EXPECT_EQ(CP_UTF8, site.encode_int());
        EXPECT_EQ(L"GET", site.method());
        EXPECT_EQ(MethodType::GET, site.method_enum());
        EXPECT_EQ(L"", site.regex());
        EXPECT_EQ(L"false", site.clickable());
        EXPECT_FALSE(site.clickable_bool());
    }
}


TEST(ConfigRepository, writeGeneral)
{
    const std::wstring iniPath(L"../UnitTest/testdata/output_writeGeneralTest.ini");

    // 準備
    {
        /// @note (yoichi) ../UnitTest/testdata/output_writeGeneralTest.ini の削除するならここ
    }

    // 書き込み
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        General general;

        general.setTripleClick(L"true");
        general.setCtrlDoubleClick(L"false");
        general.setCtrlTripleClick(L"true");
        general.setVersion(L"2.72.01");
        general.setCheckBeckyUpdate(L"false");
        general.setWarningOpenUrl(L"false");
        general.setLogLevel(L"5");

        general.setShortcut(App::Browzer, L"0");
        general.setShortcut(App::Explorer, L"1");
        general.setShortcut(App::FFFTP, L"2");
        general.setShortcut(App::FileZilla, L"3");
        general.setShortcut(App::Google, L"4");
        general.setShortcut(App::PasteBracket, L"5");
        general.setShortcut(App::RemoteDesktop, L"6");
        general.setShortcut(App::TeraTerm, L"7");
        general.setShortcut(App::TortoiseSVN, L"8");
        general.setShortcut(App::WinSCP, L"9");

        configRepository.writeGeneral(general);
    }

    // テスト
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        const General general = configRepository.readGeneral();

        EXPECT_EQ(L"true", general.tripleClick());
        EXPECT_EQ(L"false", general.ctrlDoubleClick());
        EXPECT_EQ(L"true", general.ctrlTripleClick());
        EXPECT_EQ(L"2.72.01", general.version());
        EXPECT_EQ(L"false", general.checkBeckyUpdate());
        EXPECT_EQ(L"false", general.warningOpenUrl());
        EXPECT_EQ(L"5", general.logLevel());
        EXPECT_EQ(bkzoo::log::LogLevel::Debug, general.logLevel_enum());

        EXPECT_EQ(L'0', general.shortcut(App::Browzer));
        EXPECT_EQ(L'1', general.shortcut(App::Explorer));
        EXPECT_EQ(L'2', general.shortcut(App::FFFTP));
        EXPECT_EQ(L'3', general.shortcut(App::FileZilla));
        EXPECT_EQ(L'4', general.shortcut(App::Google));
        EXPECT_EQ(L'5', general.shortcut(App::PasteBracket));
        EXPECT_EQ(L'6', general.shortcut(App::RemoteDesktop));
        EXPECT_EQ(L'7', general.shortcut(App::TeraTerm));
        EXPECT_EQ(L'8', general.shortcut(App::TortoiseSVN));
        EXPECT_EQ(L'9', general.shortcut(App::WinSCP));
    }

    // 後始末
    {
        // ../UnitTest/testdata/output_writeGeneralTest.ini の削除はやらない
    }
}


TEST(ConfigRepository, writeStandardSites)
{
    const std::wstring iniPath(L"../UnitTest/testdata/output_writeCustomsitesTest.ini");

    // 準備
    {
        /// @note (yoichi) ../UnitTest/testdata/output_writeCustomsitesTest.ini の削除するならここ
    }

    // 書き込み
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        SiteList standardSites;
        {
            // custom site
            Site site;
            site.setClickable(L"true");
            site.setEnabled(L"true");
            site.setEncode(L"Shift_JIS");
            site.setMethod(L"POST");
            site.setPreset(L"false");
            site.setRegex(L"");
            site.setShortcut(L"G");
            //            site.setStatusbar(L"");
            site.setTitle(L"Google検索");
            site.setUrl(L"http://www.google.co.jp/{}");
            standardSites.push_back(site);
        }
        {
            // preset site
            Site site;
            site.setPreset(L"true");
            site.setPresetid(L"5");
            site.setEnabled(L"true");
            standardSites.push_back(site);
        }
        {
            // custom site
            Site site;
            site.setClickable(L"false");
            site.setEnabled(L"false");
            site.setEncode(L"UTF-8");
            site.setMethod(L"GET");
            site.setPreset(L"false");
            site.setRegex(L"/.*/");
            site.setShortcut(L"Y");
            //            site.setStatusbar(L"");
            site.setTitle(L"Yahoo!検索");
            site.setUrl(L"http://www.yahoo.co.jp/{}");
            standardSites.push_back(site);
        }
        {
            // preset site
            Site site;
            site.setPreset(L"true");
            site.setPresetid(L"3");
            site.setEnabled(L"false");
            standardSites.push_back(site);
        }
        {
            // custom site
            Site site;
            site.setClickable(L"true");
            site.setEnabled(L"false");
            site.setEncode(L"UTF-8");
            site.setMethod(L"GET");
            site.setPreset(L"false");
            site.setRegex(L"/.*/");
            site.setShortcut(L"E");
            //            site.setStatusbar(L"");
            site.setTitle(L"Excite検索");
            site.setUrl(L"http://www.excite.co.jp/{}");
            standardSites.push_back(site);
        }

        configRepository.writeStandardSites(standardSites);
    }

    // 途中のテスト
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        const SiteVector standardSites = configRepository.readStandardSites();

        ASSERT_EQ(5, standardSites.size());
    }

    // 書き込み２度目（最初のを削除しているはず）
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        SiteList standardSites;
        {
            Site site;
            site.setClickable(L"true");
            site.setEnabled(L"true");
            site.setEncode(L"Shift_JIS");
            site.setMethod(L"POST");
            site.setPreset(L"false");
            site.setRegex(L"");
            site.setShortcut(L"G");
            //            site.setStatusbar(L"");
            site.setTitle(L"Google検索");
            site.setUrl(L"http://www.google.co.jp/{}");

            standardSites.push_back(site);
        }
        {
            Site site;
            site.setClickable(L"false");
            site.setEnabled(L"false");
            site.setEncode(L"UTF-8");
            site.setMethod(L"GET");
            site.setPreset(L"false");
            site.setRegex(L"/.*/");
            site.setShortcut(L"Y");
            //            site.setStatusbar(L"");
            site.setTitle(L"Yahoo!検索");
            site.setUrl(L"http://www.yahoo.co.jp/{}");

            standardSites.push_back(site);
        }
        {
            Site site;
            site.setClickable(L"true");
            site.setEnabled(L"false");
            site.setEncode(L"UTF-8");
            site.setMethod(L"GET");
            site.setPreset(L"true"); // preset==true
            site.setPresetid(L"3");
            site.setRegex(L"/.*/");
            site.setShortcut(L"E");
            //            site.setStatusbar(L"");
            site.setTitle(L"Excite検索");
            site.setUrl(L"http://www.excite.co.jp/{}");

            standardSites.push_back(site);
        }

        configRepository.writeStandardSites(standardSites);
    }

    // テスト
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        const SiteVector standardSites = configRepository.readStandardSites();

        ASSERT_EQ(3, standardSites.size());
        {
            const auto& site = standardSites.at(0);
            EXPECT_EQ(L"true", site.clickable());
            EXPECT_EQ(L"true", site.enabled());
            EXPECT_EQ(L"Shift_JIS", site.encode());
            EXPECT_EQ(L"POST", site.method());
            EXPECT_EQ(L"false", site.preset());
            EXPECT_EQ(L"", site.regex());
            EXPECT_EQ(L"G", site.shortcut());
            EXPECT_EQ(L"選択文字列をGoogle検索", site.statusbar());
            EXPECT_EQ(L"Google検索", site.title());
            EXPECT_EQ(L"http://www.google.co.jp/{}", site.url());
        }
        {
            const auto& site = standardSites.at(1);
            EXPECT_EQ(L"false", site.clickable());
            EXPECT_EQ(L"false", site.enabled());
            EXPECT_EQ(L"UTF-8", site.encode());
            EXPECT_EQ(L"GET", site.method());
            EXPECT_EQ(L"false", site.preset());
            EXPECT_EQ(L"/.*/", site.regex());
            EXPECT_EQ(L"Y", site.shortcut());
            EXPECT_EQ(L"選択文字列をYahoo!検索", site.statusbar());
            EXPECT_EQ(L"Yahoo!検索", site.title());
            EXPECT_EQ(L"http://www.yahoo.co.jp/{}", site.url());
        }
        {
            const auto& site = standardSites.at(2);
            EXPECT_EQ(L"", site.clickable());
            EXPECT_EQ(L"false", site.enabled());
            EXPECT_EQ(L"", site.encode());
            EXPECT_EQ(L"", site.method());
            EXPECT_EQ(L"true", site.preset());
            EXPECT_EQ(L"3", site.presetid());
            EXPECT_EQ(L"", site.regex());
            EXPECT_EQ(L"", site.shortcut());
            EXPECT_EQ(L"", site.statusbar());
            EXPECT_EQ(L"", site.title());
            EXPECT_EQ(L"", site.url());
        }
    }

    // 後始末
    {
        // ../UnitTest/testdata/output_writeCustomsitesTest.ini の削除はやらない
    }

}



TEST(ConfigRepository, removeStandardSites)
{
    const std::wstring iniPath(L"../UnitTest/testdata/output_removeCustomSitesTest.ini");

    // 準備
    {
        /// @note (yoichi) ../UnitTest/testdata/output_removeCustomSitesTest.ini の削除するならここ
    }

    // 書き込み
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        SiteList customSites;
        {
            Site site;
            site.setClickable(L"true");
            site.setEnabled(L"true");
            site.setEncode(L"Shift_JIS");
            site.setMethod(L"POST");
            site.setPreset(L"false");
            site.setRegex(L"");
            site.setShortcut(L"G");
            //            site.setStatusbar(L"");
            site.setTitle(L"Google検索");
            site.setUrl(L"http://www.google.co.jp/{}");

            customSites.push_back(site);
        }
        {
            Site site;
            site.setClickable(L"false");
            site.setEnabled(L"false");
            site.setEncode(L"UTF-8");
            site.setMethod(L"GET");
            site.setPreset(L"false");
            site.setRegex(L"/.*/");
            site.setShortcut(L"Y");
            //            site.setStatusbar(L"");
            site.setTitle(L"Yahoo!検索");
            site.setUrl(L"http://www.yahoo.co.jp/{}");

            customSites.push_back(site);
        }

        configRepository.writeStandardSites(customSites);
    }

    // テスト
    {
        ConfigRepository configRepository;
        const bool ret = configRepository.initialize(iniPath, L"");
        ASSERT_TRUE(ret);

        // テストデータの確認
        {
            const SiteVector standardSites = configRepository.readStandardSites();
            ASSERT_EQ(2, standardSites.size());
        }

        // 削除テスト
        {
            configRepository.removeStandardSites();

            const SiteVector standardSites = configRepository.readStandardSites();
            ASSERT_EQ(0, standardSites.size());
        }
    }

    // 後始末
    {
        // ../UnitTest/testdata/output_removeCustomSitesTest.ini の削除はやらない
    }

}
