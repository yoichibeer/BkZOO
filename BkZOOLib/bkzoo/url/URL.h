/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_URL_URL_H
#define BKZ_URL_URL_H

#include <string>

#include <regex>

#include "defs.h"

namespace bkzoo
{
    namespace url
    {

        enum Scheme : unsigned long
        {
            NON = 0x00000000,
            HTTP = 0x00000001,
            HTTPS = 0x00000002,
            FTP = 0x00000004,
            FTPS = 0x00000008,
            SFTP = 0x00000010,
            FILE_URI_LOCALPATH = 0x00000020,
            FILE_URI_UNC = 0x00000040,
            TELNET = 0x00000080,
            SVN = 0x00000100,
            SCP = 0x00000200,
            RCP = 0x00000400,
            IP_OR_DOMAIN_OR_COMUTERNAME_ONLY = 0x00000800, // for remote desktop use. (localhostはのぞく)
            MAIL = 0x00001000,
        };

        class URL final
        {
        public:
            // schemeFilter でフィルターかけて残ったものだけ有効なSchemeとする
            URL(const std::wstring& url, unsigned long schemeFilter);
            ~URL() = default;

            // validSchemes_が0でなければtrueを返却
            bool validate() const;

            // enum class Schemeの定義の昇順が優先順となり、最初にhitしたscheme文字列を付加してURLを返却
            std::wstring getURL() const;

            // File URI かつ 切り詰めるか知りたい場合の専用
            std::wstring getURLWith(bool* truncatedFileUri) const;

            // for test use only
            unsigned long getValidSchemes() const;

        private:
            const std::wstring orginalUrl_; // コンストラクタで指定する文字列保持
            const unsigned long schemeFilter_; //  validSchemes_をm_schemeFilterでフィルターかける
            unsigned long validSchemes_; // 有効なscheme種別を保持
            std::wstring urlExceptScheme_; // scheme部分を削除＆有効なURLに切り詰めたURL保持

        private:
            // 一般的なscheme指定がある場合のチェック
            bool initForHttp();
            bool initForHttps();
            bool initForFtp();
            bool initForFtps();
            bool initForSftp();
            bool initForTelnet();
            bool initForSvn();
            bool initForScp();
            bool initForRcp();
            // helper関数
            bool initialize(const std::wregex& r, Scheme scheme);


            // file: schemeの場合のチェック
            bool initForFileUriLocalpath();
            bool initForFileUriUnc();
            // helper関数
            static std::wstring validUrlExceptSchemeWithCut(const std::wstring& urlExceptScheme, bool* pCut);

            // scheme指定がない場合のチェック（scheme指定がある場合よりも優先）
            bool initForMail();

            // scheme指定がない場合のチェック（scheme指定がある場合を優先）
            bool initForIp();
            bool initForLocalhost();
            bool initForTopLevelDomain();
            bool initForPlainpath();
            bool initForOnlyComputername();
            // helper関数
            static bool is0_255range(const std::wstring& numStr);
            static bool simpleCheckScheme(const std::wstring& url);

            DISALLOW_COPY_AND_ASSIGN(URL);
        };

    }
}

#endif // BKZ_URL_URL_H
