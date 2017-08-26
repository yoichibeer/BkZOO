/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <tut/tut.hpp>
#include "bkzoo_tut.h"

#include "bkzoo_url.h"
#include "bkzoo_util.h"

#include <filesystem>
namespace fs = std::experimental::filesystem;

#include <Lmshare.h> // NetShareAdd and NetShareDel

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace url
    {

        // for test use only
        unsigned long URL::getValidSchemes() const
        {
            return validSchemes_;
        }

    }
}

namespace tut
{

    // Data used by each test
    struct URL_data
    {
        virtual ~URL_data()
        {
        }
    };

    // Test group registration
    typedef test_group<URL_data> factory;
    typedef factory::object object;

}

namespace
{

    tut::factory tf("URL");

}

namespace tut
{

    constexpr wchar_t* BKZOO_TEST_SHARE_PATH = L"C:\\bkzoo_test_share_folder";
    constexpr wchar_t* BKZOO_TEST_SHARE_FOLDER = L"C:\\bkzoo_test_share_folder\\hello";

    template<>
    template<>
    void object::test<50>()
    {
        // cleanup for URL test group

        // 削除処理は怖いのでやらない。一度だけ管理者で実行すれば、後は通常の権限で実行できるので、削除しないという理由もある。
        //{
        //    ::NetShareDel(nullptr, L"bkzoo_test_share_folder", 0);
        //    {
        //        if (fs::exists(BKZOO_TEST_SHARE_FOLDER))
        //        {
        //            fs::remove_all(BKZOO_TEST_SHARE_FOLDER);
        //            fs::remove_all(BKZOO_TEST_SHARE_PATH);
        //        }
        //    }
        //}
    }

    // HTTP
    template<>
    template<>
    void object::test<1>()
    {
        // initialize for URL test group
        {
            {
                if (!fs::exists(BKZOO_TEST_SHARE_FOLDER))
                {
                    fs::create_directories(BKZOO_TEST_SHARE_FOLDER);
                }
            }

            SHARE_INFO_2 shareInfo = { 0 };
            DWORD parm_err = 0;
            shareInfo.shi2_netname = L"bkzoo_test_share_folder";
            shareInfo.shi2_type = STYPE_DISKTREE;
            shareInfo.shi2_remark = L"TESTSHARE to test NetShareAdd";
            shareInfo.shi2_permissions = 0;
            shareInfo.shi2_max_uses = 1;
            shareInfo.shi2_current_uses = 0;
            shareInfo.shi2_path = BKZOO_TEST_SHARE_PATH;
            shareInfo.shi2_passwd = nullptr;
            if (!fs::exists(L"\\\\localhost\\bkzoo_test_share_folder"))
            {
                NET_API_STATUS ret = ::NetShareAdd(nullptr, 2, (LPBYTE)&shareInfo, &parm_err);
                if (ret == ERROR_ACCESS_DENIED)
                    std::cout << "\n一度、このテストを管理者権限で実行してください。\nまたは C:/bkzoo_test_share_folder を bkzoo_test_share_folder \nという名前で共有して C:/bkzoo_test_share_folder/hello フォルダを作成してください。";
            }
        }

        {
            std::wstring inputUrl(L"HTTP://hoge.jp:1234/abcあいうえおabc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.jp:1234/abcあいうえおabc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"HTTP://hoge.日本.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.日本.jp:1234/abc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"HTTP://hoge.にほん.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.にほん.jp:1234/abc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"HTTP://hoge.hoge@hoge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.hoge@hoge.jp:1234/abc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pas###s@hoge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho$$$ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho%%%ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho&&&ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho'''ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho(((ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho)))ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho===ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho^^^ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho~~~ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho|||ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho[[[ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho{{{ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho;;;ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho+++ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho:::ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho***ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho]]]ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho}}}ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho,,,ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho<<<ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho>>>ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"HTTP://user:pass@ho???ge.jp:1234/abc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // HTTPS
    template<>
    template<>
    void object::test<2>()
    {
        {
            std::wstring inputUrl(L"HTTPS://user:pass@aaa@@@hoge.jp/~abc/*'a_a-#a日本語aa");
            URL url(inputUrl, Scheme::HTTPS);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"https://user:pass@aaa@@@hoge.jp/~abc/*'a_a-#a日本語aa");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTPS);
        }
        {
            std::wstring inputUrl(L"HTTPs://user:pass@ho!!!ge.jp/~abc/a_a-#aaa");
            URL url(inputUrl, Scheme::HTTPS);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // FTP
    template<>
    template<>
    void object::test<3>()
    {
        {
            std::wstring inputUrl(
                L"FTP://user:pass@hoge.jp/~abc/a_a-#aa〜−¢£半角空白⇒ 全角空白⇒　タブ⇒	a");
            URL url(inputUrl, Scheme::FTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(
                L"ftp://user:pass@hoge.jp/~abc/a_a-#aa〜−¢£半角空白⇒ 全角空白⇒　タブ⇒	a");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FTP);
        }
        {
            std::wstring inputUrl(L"fTp://user:pass@hoge.jp:aaaaaaaaaaaa/~abc/{}{}{}a_a-#aaa");
            URL url(inputUrl, Scheme::FTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // FTPS
    template<>
    template<>
    void object::test<4>()
    {
        {
            std::wstring inputUrl(L"FTPS://user:pass@hoge.jp:12345/じゃぱん~abc/a_a-#aaa");
            URL url(inputUrl, Scheme::FTPS);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"ftps://user:pass@hoge.jp:12345/じゃぱん~abc/a_a-#aaa");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FTPS);
        }
        {
            std::wstring inputUrl(L"fTpS://user:pass@hoge.jp\"\"\"/~abc/a_a-#aaa[][][]");
            URL url(inputUrl, Scheme::FTPS);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);

        }
    }


    // SFTP
    template<>
    template<>
    void object::test<16>()
    {
        {
            std::wstring inputUrl(L"SFTP://user:pass@hoge.jp:12345/じゃぱん〜−~abc/a_a-#aaa");
            URL url(inputUrl, Scheme::SFTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"sftp://user:pass@hoge.jp:12345/じゃぱん〜−~abc/a_a-#aaa");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::SFTP);
        }
        {
            std::wstring inputUrl(L"SfTp://user:pass@hoge.jp\"\"\"/~abc/a_a-#aaa[][][]");
            URL url(inputUrl, Scheme::SFTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);

        }
    }


    // TELNET
    template<>
    template<>
    void object::test<5>()
    {
        {
            std::wstring inputUrl(L"TELNET://user:pass@hoge.jp/");
            URL url(inputUrl, Scheme::TELNET);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"telnet://user:pass@hoge.jp/");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::TELNET);
        }
        {
            std::wstring inputUrl(L"telneT://us###er:pass@hoge.jp");
            URL url(inputUrl, Scheme::TELNET);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // SVN
    template<>
    template<>
    void object::test<6>()
    {
        {
            std::wstring inputUrl(L"SVN://user:pass@hoge.jp/aA0;?:@&=+$,-_.!~*'()%#");
            URL url(inputUrl, Scheme::SVN);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"svn://user:pass@hoge.jp/aA0;?:@&=+$,-_.!~*'()%#");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::SVN);
        }
        {
            std::wstring inputUrl(L"svn://user:p\\\\\\ass@hoge.jp/aA0;?:@&=+$,-_.!~*'()%#<><><>_");
            URL url(inputUrl, Scheme::SVN);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // SCP
    template<>
    template<>
    void object::test<7>()
    {
        {
            std::wstring inputUrl(L"SCP://user:pass@hoge.jp/'''aA0;?:@&=+$,-_.!~*'()%#");
            URL url(inputUrl, Scheme::SCP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"scp://user:pass@hoge.jp/'''aA0;?:@&=+$,-_.!~*'()%#");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::SCP);
        }
        {
            std::wstring inputUrl(L"scp://user:pas///s@hoge.jp/```aA0;?:@&=+$,-_.!~*'()%#_");
            URL url(inputUrl, Scheme::SCP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // RCP
    template<>
    template<>
    void object::test<8>()
    {
        {
            std::wstring inputUrl(L"RCP://user:pass@hoge.jp/'''aA0;?:@&=+$,-_.!~*'()%#");
            URL url(inputUrl, Scheme::RCP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"rcp://user:pass@hoge.jp/'''aA0;?:@&=+$,-_.!~*'()%#");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(),
                static_cast<unsigned long>(Scheme::RCP));
        }
        {
            std::wstring inputUrl(L"RCp://user:pass{}{}{}@ho###ge.jp/aA0;?:@&=+$,-_.!~*'()%#_");
            URL url(inputUrl, Scheme::RCP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // FILE DRIVE
    template<>
    template<>
    void object::test<9>()
    {
        {
            std::wstring inputUrl(L"C:\\arienaipath\\goodmorning\\日本語");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:///C:\\");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"C:\\Windows\\System32");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:///C:\\Windows\\System32");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"FILE://////C:\\Windows\\System32\\arienaipath");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:///C:\\Windows\\System32");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"file:C:/Windows\\System32\\arienipath");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:///C:\\Windows\\System32");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"file:C:/Windows\\System32\\arienipath");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:///C:/Windows\\System32\\arienipath");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"file:C:/Windows\\System32\\aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:///C:\\Windows\\System32");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"file:C:\\DATA\\lab\\BkZOO\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:///C:\\DATA\\lab\\BkZOO\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"c:\\DATA\\lab\\BkZOO\\bkzoo_test_share_folder\\/hello");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:///c:\\DATA\\lab\\BkZOO\\bkzoo_test_share_folder\\hello");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"file:\\//\\//\\\\c:\\DATA\\lab\\BkZOO\\bkzoo_test_share_folder//hello");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:///c:\\DATA\\lab\\BkZOO\\bkzoo_test_share_folder\\hello");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_LOCALPATH);
        }
        {
            std::wstring inputUrl(L"C:DATA\\lab\\BkZOO\\bkzoo_test_share_folder");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"file:C:/Windows\\System32\\aA0!#$%&'()-=^~@`[{;+]},._\"\"\"");
            URL url(inputUrl, Scheme::FILE_URI_LOCALPATH);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // FILE UNC
    template<>
    template<>
    void object::test<10>()
    {
        wchar_t buffer[_MAX_PATH] = { 0 };
        DWORD size = sizeof(buffer);
        ::GetComputerNameEx(COMPUTER_NAME_FORMAT::ComputerNameNetBIOS, buffer, &size);
        const std::wstring netBiosName(buffer);

        {
            std::wstring inputUrl(L"\\\\invalid_computer_name\\abc\\にほんご");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"FILE:\\\\invalid_computer_name");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(netBiosName);
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName);
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(netBiosName).append(L"\\not_exists_path\\にほんご");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName);
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(netBiosName).append(L"\\not_exists_path\\にほんご");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName).append(L"\\not_exists_path\\にほんご");

            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder\\aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"\\\\localhost\\bkzoo_test_share_folder\\hello\\あいうえお\\");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:\\\\localhost\\bkzoo_test_share_folder\\hello\\あいうえお\\");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"\\\\arienai.domain.jp\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:\\\\arienai.domain.jp"); //\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"file:\\\\localhost\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:\\\\localhost\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"file:\\\\localhost\\");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:\\\\localhost");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, false);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"\\\\//localhost\\");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:\\\\localhost");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, false);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"file:\\\\\\localhost\\abc");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:\\\\localhost");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder\\/");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder\\");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, false);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\//\\//\\").append(netBiosName).append(L"\\\\bkzoo_test_share_folder/");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder\\");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, false);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"file:\\//\\//\\").append(netBiosName).append(L"//bkzoo_test_share_folder\\no_exist_path");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl;
            expectedUrl.append(L"file:\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"file:\\\\localhost\\D:\\デジカメ");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"\\\\localhost\\D:\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(netBiosName).append(L"\\bkzoo_test_share_folder\\aA0!#$%&'()-=^~@`[{;+]},._\"\"\"");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl;
            inputUrl.append(L"\\\\").append(L"\\bkzoo_test_share_folder\\<><><>aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"\\\\12345\\bkzoo_test_share_folder\\aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // IP
    template<>
    template<>
    void object::test<11>()
    {
        {
            std::wstring inputUrl(L"usr:pass@123.45.67.89:11111");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://usr:pass@123.45.67.89:11111");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"127.0.0.1/not_exists_path");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:\\\\127.0.0.1");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            outputUrl = url.getURL();
            expectedUrl = L"file:\\\\127.0.0.1/not_exists_path";
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"123.45.67.89:12345/aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::FTPS);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"ftps://123.45.67.89:12345/aA0!#$%&'()-=^~@`[{;+]},._");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FTPS);
        }
        {
            std::wstring inputUrl(L"127.0.0.1\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:\\\\127.0.0.1\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"\\\\127.0.0.1\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:\\\\127.0.0.1\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"file:\\\\127.0.0.1\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:\\\\127.0.0.1\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"123.45.67.89/aA0!#$%&'()-=^~@`[{;+]},._<><><>");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://123.45.67.89/aA0!#$%&'()-=^~@`[{;+]},._<><><>");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"999.999.999.999/aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"123.45.67.89a");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // localhost
    template<>
    template<>
    void object::test<12>()
    {
        {
            std::wstring inputUrl(L"LOCALHOST:11111");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://LOCALHOST:11111");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"localhost/hello");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:\\\\localhost");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"localhost:12345/aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::FTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"ftp://localhost:12345/aA0!#$%&'()-=^~@`[{;+]},._");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FTP);
        }
        {
            std::wstring inputUrl(L"localhost\\bkzoo_test_share_folder\\hello\\あいうえお");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"file:\\\\localhost\\bkzoo_test_share_folder\\hello\\あいうえお");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"localhost/aA0!#$%&'()-=^~@`[{;+]},._<><><>");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://localhost/aA0!#$%&'()-=^~@`[{;+]},._<><><>");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"localhosta");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"localhostあ");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // TOP LEVEL DOMAIN
    template<>
    template<>
    void object::test<13>()
    {
        {
            std::wstring inputUrl(
                L"a0\"$%&'()-=^~|@`[{;+:*]},<.>_@abc.def.com:123/aA0!#$%&'()-=^~@`[{;+]},._");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(
                L"http://a0\"$%&'()-=^~|@`[{;+:*]},<.>_@abc.def.com:123/aA0!#$%&'()-=^~@`[{;+]},._");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"user:pass@aaaaaaaaaaa.ZW:123/aaaaaaaa\\aaaa\\a\\a\\a\\");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(
                L"http://user:pass@aaaaaaaaaaa.ZW:123/aaaaaaaa\\aaaa\\a\\a\\a\\");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"user:pass@aaaaaaaaaaa.ZW:123");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://user:pass@aaaaaaaaaaa.ZW:123");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"aaaaaaaaaaa.ZW:123");
            URL url(inputUrl, Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"aaaaaaaaaaa.ZW:123");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
        }
        {
            std::wstring inputUrl(L"@aaa.co.jp");
            URL url(inputUrl, Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"user:pass@aaaaaaaaaaa.ZWAAA:123/aaaaaaaa\\aaaa\\a\\a\\a\\");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(
                L"a0\"$%&'()-=^~|@`[{;+:*]},###<.>_@abc.def.com:123/aA0!#$%&'()-=^~@`[{;+]},._<><><>");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // PLAIN PATH
    template<>
    template<>
    void object::test<14>()
    {
        {
            std::wstring inputUrl(L"aA0-_.\\!\"#$%&'()-=^~|@`[{:*]},<.>/?");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://aA0-_.\\!\"#$%&'()-=^~|@`[{:*]},<.>/?");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"a/");
            URL url(inputUrl, Scheme::FILE_URI_UNC);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            bool truncated = false;
            std::wstring outputUrl = url.getURLWith(&truncated);
            std::wstring expectedUrl(L"file:\\\\a");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);
            ensure_equals(M(__LINE__), truncated, true);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FILE_URI_UNC);
        }
        {
            std::wstring inputUrl(L"aA0-_.???/!\"#$%&'()-=^~|@`[{:*]},<.>/?");
            URL url(inputUrl, Scheme::TELNET);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
    }


    // COMUPTER NAME
    template<>
    template<>
    void object::test<15>()
    {
        {
            std::wstring inputUrl(L"-0CR-Zcr-z0");
            URL url(inputUrl, Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"-0CR-Zcr-z0");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
        }
        {
            std::wstring inputUrl(L"cr_z");
            URL url(inputUrl, Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"12345");
            URL url(inputUrl, Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(),
                static_cast<unsigned long>(Scheme::NON));
        }
    }


    // Error
    template<>
    template<>
    void object::test<17>()
    {
        {
            std::wstring inputUrl(L"表abc祖");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
        }
        {
            std::wstring inputUrl(L"GoogleSearch");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), !validUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), 0UL);
            try
            {
                url.getURL();
            }
            catch (std::exception& e)
            {
                const std::string errorMessage(e.what());
                ensure_equals(M(__LINE__), errorMessage, "invalid: validSchemes_");
                return;
            }
            fail(M(__LINE__));
        }
    }


    // mail
    template<>
    template<>
    void object::test<18>()
    {
        {
            std::wstring inputUrl(L"a@aaa.co.jp");
            URL url(inputUrl, Scheme::MAIL);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl = L"a@aaa.co.jp";
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::MAIL);
        }
        {
            std::wstring inputUrl(L"a@127.0.0.1");
            URL url(inputUrl, Scheme::FTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"ftp://a@127.0.0.1");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::FTP);
        }
    }

    // 文字列先頭の引用符
    template<>
    template<>
    void object::test<19>()
    {
        {
            std::wstring inputUrl(L"> > HTTP://hoge.jp:1234/abcあいうえおabc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.jp:1234/abcあいうえおabc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"> | HTTP://hoge.jp:1234/abcあいうえおabc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.jp:1234/abcあいうえおabc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L"||HTTP://hoge.jp:1234/abcあいうえおabc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.jp:1234/abcあいうえおabc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }
        {
            std::wstring inputUrl(L">> HTTP://hoge.jp:1234/abcあいうえおabc");
            URL url(inputUrl, Scheme::HTTP);
            bool validUrl = url.validate();
            ensure(M(__LINE__), validUrl);

            std::wstring outputUrl = url.getURL();
            std::wstring expectedUrl(L"http://hoge.jp:1234/abcあいうえおabc");
            ensure_equals(M(__LINE__), outputUrl, expectedUrl);

            ensure_equals(M(__LINE__), url.getValidSchemes(), Scheme::HTTP);
        }

    }
}
