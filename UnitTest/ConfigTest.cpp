/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <gtest/gtest.h>

#include "config/Config.h"

#include "bkzoo_string.h"

#include <algorithm>

#include "debug/detect_memory_leak.h"

using namespace bkzoo::config;

TEST(General, shortcuts)
{
    General general;

    // default
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

    // Browzer
    general.setShortcut(App::Browzer, L"B");
    EXPECT_EQ(L'B', general.shortcut(App::Browzer));

    // Explorer
    general.setShortcut(App::Explorer, L"E");
    EXPECT_EQ(L'E', general.shortcut(App::Explorer));

    // FFFTP
    general.setShortcut(App::FFFTP, L"F");
    EXPECT_EQ(L'F', general.shortcut(App::FFFTP));

    // FileZilla
    general.setShortcut(App::FileZilla, L"F");
    EXPECT_EQ(L'F', general.shortcut(App::FileZilla));

    // Google
    general.setShortcut(App::Google, L"Google");
    EXPECT_EQ(L'G', general.shortcut(App::Google));

    // PasteBracket
    general.setShortcut(App::PasteBracket, L"P");
    EXPECT_EQ(L'P', general.shortcut(App::PasteBracket));

    // RemoteDesktop
    general.setShortcut(App::RemoteDesktop, L"R");
    EXPECT_EQ(L'R', general.shortcut(App::RemoteDesktop));

    // TeraTerm
    general.setShortcut(App::TeraTerm, L"R");
    EXPECT_EQ(L'R', general.shortcut(App::TeraTerm));

    // TortoiseSVN
    general.setShortcut(App::TortoiseSVN, L"T");
    EXPECT_EQ(L'T', general.shortcut(App::TortoiseSVN));

    // WinSCP
    general.setShortcut(App::WinSCP, L"WinSCP");
    EXPECT_EQ(L'W', general.shortcut(App::WinSCP));

    // 小文字
    general.setShortcut(App::WinSCP, L"a");
    EXPECT_EQ(L'\0', general.shortcut(App::WinSCP));

    // 数字
    general.setShortcut(App::WinSCP, L"0");
    EXPECT_EQ(L'0', general.shortcut(App::WinSCP));

    // 記号
    general.setShortcut(App::WinSCP, L"!");
    EXPECT_EQ(L'\0', general.shortcut(App::WinSCP));

    // 空文字
    general.setShortcut(App::WinSCP, L"");
    EXPECT_EQ(L'\0', general.shortcut(App::WinSCP));

    // 全角文字
    general.setShortcut(App::WinSCP, L"全角");
    EXPECT_EQ(L'\0', general.shortcut(App::WinSCP));
}


TEST(General, tripleClick)
{
    General general;

    // default
    EXPECT_EQ(L"", general.tripleClick());
    EXPECT_TRUE(general.tripleClick_bool());

    // true
    general.setTripleClick(L"true");
    EXPECT_TRUE(general.tripleClick_bool());

    // false
    general.setTripleClick(L"false");
    EXPECT_FALSE(general.tripleClick_bool());

    // それ以外
    general.setTripleClick(L"TrUe");
    EXPECT_FALSE(general.tripleClick_bool());
}


TEST(General, ctrlDoubleClick)
{
    General general;

    // default
    EXPECT_EQ(L"", general.ctrlDoubleClick());
    EXPECT_TRUE(general.ctrlDoubleClick_bool());

    // true
    general.setCtrlDoubleClick(L"true");
    EXPECT_TRUE(general.ctrlDoubleClick_bool());

    // false
    general.setCtrlDoubleClick(L"false");
    EXPECT_FALSE(general.ctrlDoubleClick_bool());

    // それ以外
    general.setCtrlDoubleClick(L"TrUe");
    EXPECT_FALSE(general.ctrlDoubleClick_bool());
}


TEST(General, ctrlTripleClick)
{
    General general;

    // default
    EXPECT_EQ(L"", general.ctrlTripleClick());
    EXPECT_TRUE(general.ctrlTripleClick_bool());

    // true
    general.setCtrlTripleClick(L"true");
    EXPECT_TRUE(general.ctrlTripleClick_bool());

    // false
    general.setCtrlTripleClick(L"false");
    EXPECT_FALSE(general.ctrlTripleClick_bool());

    // それ以外
    general.setCtrlTripleClick(L"TrUe");
    EXPECT_FALSE(general.ctrlTripleClick_bool());
}


TEST(General, version)
{
    General general;

    // default
    EXPECT_EQ(L"", general.version());

    // 正常系
    general.setVersion(L"VERSION1.0");
    EXPECT_EQ(L"VERSION1.0", general.version());

    // 全角もいける
    general.setVersion(L"全角文字列");
    EXPECT_EQ(L"全角文字列", general.version());
}


TEST(General, checkBeckyUpdate)
{
    General general;

    // default
    EXPECT_EQ(L"", general.checkBeckyUpdate());
    EXPECT_TRUE(general.checkBeckyUpdate_bool());

    // 正常系
    general.setCheckBeckyUpdate(L"false");
    EXPECT_FALSE(general.checkBeckyUpdate_bool());

    // 正常系
    general.setCheckBeckyUpdate(L"true");
    EXPECT_TRUE(general.checkBeckyUpdate_bool());
}


TEST(General, warningOpenUrl)
{
    General general;

    // default
    EXPECT_EQ(L"", general.warningOpenUrl());
    EXPECT_TRUE(general.warningOpenUrl_bool());

    // true
    general.setWarningOpenUrl(L"true");
    EXPECT_TRUE(general.warningOpenUrl_bool());

    // false
    general.setWarningOpenUrl(L"false");
    EXPECT_FALSE(general.warningOpenUrl_bool());

    // それ以外
    general.setWarningOpenUrl(L"TrUe");
    EXPECT_FALSE(general.warningOpenUrl_bool());
}


TEST(Site, presetid)
{
    Site site;

    // default
//    EXPECT_EQ(L"", site.presetid()); // assertで止まるのでコメントアウト

    // 正常系
    site.setPresetid(L"1");
    EXPECT_EQ(L"1", site.presetid());
    EXPECT_EQ(1, site.presetid_int());

    // 複数桁
    site.setPresetid(L"123");
    EXPECT_EQ(123, site.presetid_int());

    // 負数も一応対応
    site.setPresetid(L"-5");
    EXPECT_EQ(-5, site.presetid_int());

    // ちょっと変だけど正常
    site.setPresetid(L"00123");
    EXPECT_EQ(123, site.presetid_int());
}


TEST(Site, preset)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.preset());
    EXPECT_FALSE(site.preset_bool());

    // true
    site.setPreset(L"true");
    EXPECT_EQ(L"true", site.preset());
    EXPECT_TRUE(site.preset_bool());

    // false
    site.setPreset(L"false");
    EXPECT_FALSE(site.preset_bool());

    // それ以外
    site.setPreset(L"TrUe");
    EXPECT_FALSE(site.preset_bool());
}


TEST(Site, enabled)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.enabled());
    EXPECT_TRUE(site.enabled_bool());

    // true
    site.setEnabled(L"true");
    EXPECT_EQ(L"true", site.enabled());
    EXPECT_TRUE(site.enabled_bool());

    // false
    site.setEnabled(L"false");
    EXPECT_FALSE(site.enabled_bool());

    // それ以外
    site.setEnabled(L"TRUE");
    EXPECT_FALSE(site.enabled_bool());
}


TEST(Site, title_AND_statusbar)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.title());
    EXPECT_EQ(L"", site.statusbar());

    // title正常系
    site.setTitle(L"Google検索");
    EXPECT_EQ(L"Google検索", site.title());

    // status正常系
//    site.setStatusbar(L"");
    EXPECT_EQ(L"選択文字列をGoogle検索", site.statusbar());

    // statusbar指定あり（titleは無視）
    site.setTitle(L"Yahoo!検索");
    //    site.setStatusbar(L"選択文字列をGoogle検索");
    EXPECT_EQ(L"選択文字列をYahoo!検索", site.statusbar());

    // statusbar指定あり（titleは無視）
    site.setTitle(L"");
    //    site.setStatusbar(L"選択文字列をGoogle検索");
    EXPECT_EQ(L"", site.statusbar());
}


TEST(Site, shortcut)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.shortcut());
    EXPECT_EQ(L'\0', site.shortcut_char());

    // 正常系
    site.setShortcut(L"G");
    EXPECT_EQ(L"G", site.shortcut());
    EXPECT_EQ(L'G', site.shortcut_char());

    // 一文字以上
    site.setShortcut(L"Yahoo!");
    EXPECT_EQ(L"Y", site.shortcut());
    EXPECT_EQ(L'Y', site.shortcut_char());
}


TEST(Site, url)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.url());

    // 正常系
    site.setUrl(L"http://www.google.co.jp/{}");
    EXPECT_EQ(L"http://www.google.co.jp/{}", site.url());
}


TEST(Site, encode)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.encode());
    EXPECT_EQ(CP_ACP, site.encode_int());

    // Shift_JIS
    site.setEncode(L"Shift_JIS");
    EXPECT_EQ(L"Shift_JIS", site.encode());
    EXPECT_EQ(CP_ACP, site.encode_int());

    // UTF-8
    site.setEncode(L"UTF-8");
    EXPECT_EQ(CP_UTF8, site.encode_int());

    // EUC-JP
    site.setEncode(L"EUC-JP");
    EXPECT_EQ(CP51932, site.encode_int());

    // その他
    site.setEncode(L"エンコード");
    EXPECT_EQ(CP_ACP, site.encode_int());

    // 小文字は非対応
    site.setEncode(L"utf-8");
    EXPECT_EQ(CP_ACP, site.encode_int());
}


TEST(Site, method)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.method());
    EXPECT_EQ(MethodType::GET, site.method_enum());

    // GET
    site.setMethod(L"GET");
    EXPECT_EQ(L"GET", site.method());
    EXPECT_EQ(MethodType::GET, site.method_enum());

    // POST
    site.setMethod(L"POST");
    EXPECT_EQ(MethodType::POST, site.method_enum());

    // それ以外
    site.setMethod(L"post");
    EXPECT_EQ(MethodType::GET, site.method_enum());
}


TEST(Site, regex)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.regex());

    // 正常系
    site.setRegex(L"^[ -~｡-ﾟ]*$");
    EXPECT_EQ(L"^[ -~｡-ﾟ]*$", site.regex());
}


TEST(Site, clickable)
{
    Site site;

    // default
    EXPECT_EQ(L"", site.clickable());
    EXPECT_FALSE(site.clickable_bool());

    // true
    site.setClickable(L"true");
    EXPECT_EQ(L"true", site.clickable());
    EXPECT_TRUE(site.clickable_bool());

    // false
    site.setClickable(L"false");
    EXPECT_FALSE(site.clickable_bool());

    // それ以外
    site.setClickable(L"TrUe");
    EXPECT_FALSE(site.clickable_bool());
}



class ConfigTest : public ::testing::Test
{
protected:
    // virtual void SetUp() {}
    // virtual void TearDown() {}

    Config config_;
};



TEST_F(ConfigTest, config)
{
    // テストデータ作成
    {
        General general;
        general.setTripleClick(L"true");
        general.setCtrlDoubleClick(L"false");
        general.setCtrlTripleClick(L"true");
        general.setVersion(L"1.2.3");
        general.setCheckBeckyUpdate(L"false");
        general.setWarningOpenUrl(L"false");

        general.setShortcut(App::Browzer, L"B");
        general.setShortcut(App::Explorer, L"E");
        general.setShortcut(App::FFFTP, L"F");
        general.setShortcut(App::FileZilla, L"F");
        general.setShortcut(App::Google, L"G");
        general.setShortcut(App::PasteBracket, L"P");
        general.setShortcut(App::RemoteDesktop, L"R");
        general.setShortcut(App::TeraTerm, L"T");
        general.setShortcut(App::TortoiseSVN, L"T");
        general.setShortcut(App::WinSCP, L"W");

        config_.setGeneral(general);
    }
    {
        std::list<Site> sites;
        {
            Site site;
            site.setClickable(L"true");
            site.setEnabled(L"true");
            site.setEncode(L"Shift_JIS");
            site.setPresetid(L"1");
            site.setMethod(L"POST");
            site.setPreset(L"true");
            site.setRegex(L"");
            site.setShortcut(L"G");
            //            site.setStatusbar(L"");
            site.setTitle(L"Google検索");
            site.setUrl(L"http://www.google.co.jp/{}");

            sites.push_back(site);
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

            sites.push_back(site);
        }

        config_.setSites(sites);
    }

    // 以下、テスト

    EXPECT_TRUE(config_.tripleClick());
    EXPECT_FALSE(config_.ctrlDoubleClick());
    EXPECT_TRUE(config_.ctrlTripleClick());
    EXPECT_EQ(L"1.2.3", config_.version());
    EXPECT_FALSE(config_.checkBeckyUpdate());
    EXPECT_FALSE(config_.warningOpenUrl());

    EXPECT_EQ(L'B', config_.shortcut(App::Browzer));
    EXPECT_EQ(L'E', config_.shortcut(App::Explorer));
    EXPECT_EQ(L'F', config_.shortcut(App::FFFTP));
    EXPECT_EQ(L'F', config_.shortcut(App::FileZilla));
    EXPECT_EQ(L'G', config_.shortcut(App::Google));
    EXPECT_EQ(L'P', config_.shortcut(App::PasteBracket));
    EXPECT_EQ(L'R', config_.shortcut(App::RemoteDesktop));
    EXPECT_EQ(L'T', config_.shortcut(App::TeraTerm));
    EXPECT_EQ(L'T', config_.shortcut(App::TortoiseSVN));
    EXPECT_EQ(L'W', config_.shortcut(App::WinSCP));

    std::list<Site> sites = config_.sites();
    EXPECT_EQ(2, sites.size());
    {
        auto site = sites.begin();
        std::advance(site, 0);
        EXPECT_TRUE(site->clickable_bool());
        EXPECT_TRUE(site->enabled_bool());
        EXPECT_EQ(CP_ACP, site->encode_int());
        EXPECT_EQ(1, site->presetid_int());
        EXPECT_EQ(MethodType::POST, site->method_enum());
        EXPECT_TRUE(site->preset_bool());
        EXPECT_EQ(L"", site->regex());
        EXPECT_EQ(L'G', site->shortcut_char());
        EXPECT_EQ(L"選択文字列をGoogle検索", site->statusbar());
        EXPECT_EQ(L"Google検索", site->title());
        EXPECT_EQ(L"http://www.google.co.jp/{}", site->url());
    }
    {
        auto site = sites.begin();
        std::advance(site, 1);
        EXPECT_FALSE(site->clickable_bool());
        EXPECT_FALSE(site->enabled_bool());
        EXPECT_EQ(CP_UTF8, site->encode_int());
        EXPECT_EQ(MethodType::GET, site->method_enum());
        EXPECT_FALSE(site->preset_bool());
        EXPECT_EQ(L"/.*/", site->regex());
        EXPECT_EQ(L'Y', site->shortcut_char());
        EXPECT_EQ(L"選択文字列をYahoo!検索", site->statusbar());
        EXPECT_EQ(L"Yahoo!検索", site->title());
        EXPECT_EQ(L"http://www.yahoo.co.jp/{}", site->url());
    }
}
