#include <gtest/gtest.h>

#include "config/ConfigFactory.h"
#include "bkzoo_string.h"

using namespace bkzoo;

TEST(CofigFactory, create_default)
{
    //
    // テストデータ作成
    //

    // general
    config::General general;

    // presetSites
    config::SiteMap presetSites;

    // standardSites
    config::SiteVector standardSites;


    //
    // テスト対象
    //
    std::unique_ptr<config::Config> config = config::ConfigFactory::create(
        general,
        presetSites,
        standardSites
    );


    //
    // テスト
    //
    EXPECT_TRUE(config->tripleClick());
    EXPECT_TRUE(config->ctrlDoubleClick());
    EXPECT_EQ(L"", config->version());
    EXPECT_TRUE(config->checkBeckyUpdate());
    EXPECT_TRUE(config->warningOpenUrl());

    EXPECT_EQ(L'1', config->shortcut(config::App::Browzer));
    EXPECT_EQ(L'3', config->shortcut(config::App::Explorer));
    EXPECT_EQ(L'T', config->shortcut(config::App::FFFTP));
    EXPECT_EQ(L'Z', config->shortcut(config::App::FileZilla));
    EXPECT_EQ(L'G', config->shortcut(config::App::Google));
    EXPECT_EQ(L'K', config->shortcut(config::App::PasteBracket));
    EXPECT_EQ(L'R', config->shortcut(config::App::RemoteDesktop));
    EXPECT_EQ(L'M', config->shortcut(config::App::TeraTerm));
    EXPECT_EQ(L'V', config->shortcut(config::App::TortoiseSVN));
    EXPECT_EQ(L'W', config->shortcut(config::App::WinSCP));

    ASSERT_EQ(0U, config->sites().size());
}

TEST(ConfigFactory, create)
{
    //
    // テストデータ作成
    //

    // general
    config::General general;
    general.setTripleClick(L"false");
    general.setCtrlDoubleClick(L"true");
    general.setVersion(L"1.2.3");
    general.setCheckBeckyUpdate(L"false");
    general.setWarningOpenUrl(L"false");
    general.setLogLevel(L"4");

    general.setShortcut(config::App::Browzer, L"R");
    general.setShortcut(config::App::Explorer, L"X");
    general.setShortcut(config::App::FFFTP, L"F");
    general.setShortcut(config::App::FileZilla, L"I");
    general.setShortcut(config::App::Google, L"O");
    general.setShortcut(config::App::PasteBracket, L"A");
    general.setShortcut(config::App::RemoteDesktop, L"E");
    general.setShortcut(config::App::TeraTerm, L"E");
    general.setShortcut(config::App::TortoiseSVN, L"O");
    general.setShortcut(config::App::WinSCP, L"I");

    // presetSites
    config::SiteMap presetSites;
    {
        config::Site site;
        site.setClickable(L"true");
        site.setEnabled(L"true");
        site.setEncode(L"EUC-JP");
        site.setPresetid(L"0");
        site.setMethod(L"POST");
        site.setPreset(L"true");
        site.setRegex(L"/.*/");
        site.setShortcut(L"Z");
        //        site.setStatusbar(L"");
        site.setTitle(L"Googleけんさく");
        site.setUrl(L"https://www.google.com/{}");
        presetSites[0] = site;
    }
    {
        config::Site site;
        site.setClickable(L"false");
        site.setEnabled(L"false");
        site.setEncode(L"Shift_JIS");
        site.setPresetid(L"1");
        site.setMethod(L"GET");
        site.setPreset(L"true");
        site.setRegex(L"");
        site.setShortcut(L"Y");
        //        site.setStatusbar(L"");
        site.setTitle(L"Yahoo!けんさく");
        site.setUrl(L"https://www.yahoo.com/{}");
        presetSites[1] = site;
    }
    {
        config::Site site;
        site.setClickable(L"true");
        site.setEnabled(L"true");
        site.setEncode(L"UTF-8");
        site.setPresetid(L"2");
        site.setMethod(L"POST");
        site.setPreset(L"true");
        site.setRegex(L"/excite/");
        site.setShortcut(L"X");
        //        site.setStatusbar(L"");
        site.setTitle(L"Exciteけんさく");
        site.setUrl(L"https://www.excite.com/{}");
        presetSites[2] = site;
    }
    {
        config::Site site;
        site.setClickable(L"false");
        site.setEnabled(L"true");
        site.setEncode(L"UTF-8");
        site.setPresetid(L"3");
        site.setMethod(L"GET");
        site.setPreset(L"true");
        site.setRegex(L"/goo/");
        site.setShortcut(L"W");
        //        site.setStatusbar(L"");
        site.setTitle(L"gooけんさく");
        site.setUrl(L"https://www.goo.com/{}");
        presetSites[3] = site;
    }

    // standardSites
    config::SiteVector standardSites;
    {
        // preset site 1
        config::Site site;
        site.setPresetid(L"1");
        site.setEnabled(L"false");
        site.setPreset(L"true");
        standardSites.push_back(site);
    }
    {
        // custom site
        config::Site site;
        site.setClickable(L"false");
        site.setEnabled(L"false");
        site.setEncode(L"UTF-8");
        site.setMethod(L"POST");
        site.setPreset(L"false");
        site.setRegex(L"");
        site.setShortcut(L"B");
        //        site.setStatusbar(L"");
        site.setTitle(L"BBBけんさく");
        site.setUrl(L"https://www.bbb.com/{}");
        standardSites.push_back(site);
    }
    {
        // custom site
        config::Site site;
        site.setClickable(L"true");
        site.setEnabled(L"true");
        site.setEncode(L"EUC-JP");
        site.setMethod(L"GET");
        site.setPreset(L"false");
        site.setRegex(L"/.*/");
        site.setShortcut(L"A");
        //        site.setStatusbar(L"");
        site.setTitle(L"AAAけんさく");
        site.setUrl(L"https://www.aaa.com/{}");
        standardSites.push_back(site);
    }
    {
        // preset site 2
        config::Site site;
        site.setPresetid(L"2");
        site.setEnabled(L"true");
        site.setPreset(L"true");
        standardSites.push_back(site);
    }
    {
        // preset site 3
        config::Site site;
        site.setPresetid(L"3");
        site.setEnabled(L"false");
        site.setPreset(L"true");
        standardSites.push_back(site);
    }
    {
        // preset site 0
        config::Site site;
        site.setPresetid(L"0");
        site.setEnabled(L"true");
        site.setPreset(L"true");
        standardSites.push_back(site);
    }
    {
        // custom site
        config::Site site;
        site.setClickable(L"false");
        site.setEnabled(L"true");
        site.setEncode(L"Shift_JIS");
        site.setMethod(L"GET");
        site.setPreset(L"false");
        site.setRegex(L"/ccc/");
        site.setShortcut(L"C");
        //        site.setStatusbar(L"");
        site.setTitle(L"CCCけんさく");
        site.setUrl(L"https://www.ccc.com/{}");
        standardSites.push_back(site);
    }


    //
    // テスト対象
    //
    std::unique_ptr<config::Config> config = config::ConfigFactory::create(
        general,
        presetSites,
        standardSites
    );


    //
    // テスト
    //
    EXPECT_FALSE(config->tripleClick());
    EXPECT_TRUE(config->ctrlDoubleClick());
    EXPECT_EQ(L"1.2.3", config->version());
    EXPECT_FALSE(config->checkBeckyUpdate());
    EXPECT_FALSE(config->warningOpenUrl());
    EXPECT_EQ(bkzoo::log::LogLevel::Info, config->logLevel());

    EXPECT_EQ(L'R', config->shortcut(config::App::Browzer));
    EXPECT_EQ(L'X', config->shortcut(config::App::Explorer));
    EXPECT_EQ(L'F', config->shortcut(config::App::FFFTP));
    EXPECT_EQ(L'I', config->shortcut(config::App::FileZilla));
    EXPECT_EQ(L'O', config->shortcut(config::App::Google));
    EXPECT_EQ(L'A', config->shortcut(config::App::PasteBracket));
    EXPECT_EQ(L'E', config->shortcut(config::App::RemoteDesktop));
    EXPECT_EQ(L'E', config->shortcut(config::App::TeraTerm));
    EXPECT_EQ(L'O', config->shortcut(config::App::TortoiseSVN));
    EXPECT_EQ(L'I', config->shortcut(config::App::WinSCP));
    ASSERT_EQ(7U, config->sites().size());
    {
        auto site = config->sites().begin();
        std::advance(site, 0);
        EXPECT_FALSE(site->clickable_bool());
        EXPECT_FALSE(site->enabled_bool());
        EXPECT_EQ(CP_ACP, site->encode_int());
        EXPECT_EQ(1, site->presetid_int());
        EXPECT_EQ(config::MethodType::GET, site->method_enum());
        EXPECT_TRUE(site->preset_bool());
        EXPECT_EQ(L"", site->regex());
        EXPECT_EQ(L'Y', site->shortcut_char());
        EXPECT_EQ(L"選択文字列をYahoo!けんさく", site->statusbar());
        EXPECT_EQ(L"Yahoo!けんさく", site->title());
        EXPECT_EQ(L"https://www.yahoo.com/{}", site->url());
    }
    {
        auto site = config->sites().begin();
        std::advance(site, 1);
        EXPECT_FALSE(site->clickable_bool());
        EXPECT_FALSE(site->enabled_bool());
        EXPECT_EQ(CP_UTF8, site->encode_int());
        EXPECT_EQ(config::MethodType::POST, site->method_enum());
        EXPECT_FALSE(site->preset_bool());
        EXPECT_EQ(L"", site->regex());
        EXPECT_EQ(L'B', site->shortcut_char());
        EXPECT_EQ(L"選択文字列をBBBけんさく", site->statusbar());
        EXPECT_EQ(L"BBBけんさく", site->title());
        EXPECT_EQ(L"https://www.bbb.com/{}", site->url());
    }
    {
        auto site = config->sites().begin();
        std::advance(site, 5);
        EXPECT_TRUE(site->clickable_bool());
        EXPECT_TRUE(site->enabled_bool());
        EXPECT_EQ(CP51932, site->encode_int());
        EXPECT_EQ(0, site->presetid_int());
        EXPECT_EQ(config::MethodType::POST, site->method_enum());
        EXPECT_TRUE(site->preset_bool());
        EXPECT_EQ(L"/.*/", site->regex());
        EXPECT_EQ(L'Z', site->shortcut_char());
        EXPECT_EQ(L"選択文字列をGoogleけんさく", site->statusbar());
        EXPECT_EQ(L"Googleけんさく", site->title());
        EXPECT_EQ(L"https://www.google.com/{}", site->url());
    }
    {
        auto site = config->sites().begin();
        std::advance(site, 6);
        EXPECT_FALSE(site->clickable_bool());
        EXPECT_TRUE(site->enabled_bool());
        EXPECT_EQ(CP_ACP, site->encode_int());
        EXPECT_EQ(config::MethodType::GET, site->method_enum());
        EXPECT_FALSE(site->preset_bool());
        EXPECT_EQ(L"/ccc/", site->regex());
        EXPECT_EQ(L'C', site->shortcut_char());
        EXPECT_EQ(L"選択文字列をCCCけんさく", site->statusbar());
        EXPECT_EQ(L"CCCけんさく", site->title());
        EXPECT_EQ(L"https://www.ccc.com/{}", site->url());
    }
}
