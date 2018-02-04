/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "URL.h"

#include "URLEncode.h"
#include "bkzoo_util.h"
#include "bkzoo_string.h"

#include "bkzoo_log.h"

#include <filesystem>

#include <string>
#include <sstream>
#include <regex>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace url
    {

        URL::URL(const std::wstring& url, unsigned long schemeFilter)
            : orginalUrl_(url), schemeFilter_(schemeFilter), validSchemes_(0UL)
        {
            if (initForMail()) return;
            if (initForHttp()) return;
            if (initForHttps()) return;
            if (initForFtp()) return;
            if (initForFtps()) return;
            if (initForSftp()) return;
            if (initForTelnet()) return;
            if (initForSvn()) return;
            if (initForScp()) return;
            if (initForRcp()) return;
            if (initForFileUriLocalpath()) return;
            if (initForFileUriUnc()) return;
            if (initForIp()) return;
            if (initForLocalhost()) return;
            if (initForTopLevelDomain()) return;
            if (initForPlainpath()) return;
            if (initForOnlyComputername()) return;
        }

        bool URL::validate() const
        {
            if (validSchemes_ == 0UL)
            {
                return false;
            }
            return true;
        }

        std::wstring URL::getURLWith(bool* truncatedFileUri) const
        {
            if (validSchemes_ == 0UL)
            {
                LOG_ERROR << "validSchemes_ == 0UL";
                throw std::runtime_error("invalid: validSchemes_");
            }

            std::wostringstream os;
            if ((validSchemes_ & Scheme::HTTP) != 0UL)
            {
                os << L"http://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::HTTPS) != 0UL)
            {
                os << L"https://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::FTP) != 0UL)
            {
                os << L"ftp://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::FTPS) != 0UL)
            {
                os << L"ftps://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::SFTP) != 0UL)
            {
                os << L"sftp://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::FILE_URI_LOCALPATH) != 0UL)
            {
                os << "file:///" << validUrlExceptSchemeWithCut(urlExceptScheme_, truncatedFileUri);
            }
            else if ((validSchemes_ & Scheme::FILE_URI_UNC) != 0UL)
            {
                // scheme指定がないipやlocalhostのために、ここで\\を文字列の最初に付加
                std::wstring validUrlExceptScheme(urlExceptScheme_);
                if (validUrlExceptScheme.length() < 2)
                {
                    LOG_ERROR << "validUrlExceptScheme.length()=" << validUrlExceptScheme.length();
                    throw std::runtime_error("program error: (2 > validUrlExceptScheme.length())");
                }
                const std::wstring firstSecond = validUrlExceptScheme.substr(0, 2);
                if (L"\\\\" != firstSecond)
                {
                    validUrlExceptScheme.insert(0, L"\\\\");
                }

                os << "file:" << validUrlExceptSchemeWithCut(validUrlExceptScheme, truncatedFileUri);
            }
            else if ((validSchemes_ & Scheme::TELNET) != 0UL)
            {
                os << L"telnet://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::SVN) != 0UL)
            {
                os << L"svn://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::SCP) != 0UL)
            {
                os << L"scp://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::RCP) != 0UL)
            {
                os << L"rcp://" << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY) != 0UL)
            {
                os << urlExceptScheme_;
            }
            else if ((validSchemes_ & Scheme::MAIL) != 0UL)
            {
                os << urlExceptScheme_;
            }
            else
            {
                LOG_ERROR << "program error: validSchemes_";
                throw std::runtime_error("program error: validSchemes_");
            }

            return os.str();
        }

        std::wstring URL::getURL() const
        {
            return getURLWith(nullptr);
        }

        // http
        bool URL::initForHttp()
        {
            std::wregex regex(
                L"^<?([hH][tT]|[tT])?[tT][pP]:///*([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::HTTP);
        }

        // https
        bool URL::initForHttps()
        {
            std::wregex regex(
                L"^<?([hH][tT]|[tT])?[tT][pP][sS]:///*([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::HTTPS);
        }

        // ftp://
        bool URL::initForFtp()
        {
            std::wregex regex(
                L"^<?([fF][tT][pP]:///*)([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::FTP);
        }

        // ftps://
        bool URL::initForFtps()
        {
            std::wregex regex(
                L"^<?([fF][tT][pP][sS]:///*)([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::FTPS);
        }

        // sftp://
        bool URL::initForSftp()
        {
            std::wregex regex(
                L"^<?([sS][fF][tT][pP]:///*)([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::SFTP);
        }

        // telnet://
        bool URL::initForTelnet()
        {
            std::wregex regex(
                L"^<?([tT][eE][lL][nN][eE][tT]:///*)([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::TELNET);
        }

        // svn://
        bool URL::initForSvn()
        {
            std::wregex regex(
                L"^<?([sS][vV][nN]:///*)([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::SVN);
        }

        // scp://
        bool URL::initForScp()
        {
            std::wregex regex(
                L"^<?([sS][cC][pP]:///*)([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::SCP);
        }

        // rcp://
        bool URL::initForRcp()
        {
            std::wregex regex(
                L"^<?([rR][cC][pP]:///*)([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)(:[0-9]+)?([\\\\/][^>]*)?>?$");
            return initialize(regex, Scheme::RCP);
        }

        // helper
        bool URL::initialize(const std::wregex& regex, Scheme scheme)
        {
            std::wsmatch results;
            std::wstring resultsStr[4];
            if (std::regex_search(orginalUrl_, results, regex))
            {
                for (int i = 0; i < 4; ++i)
                    resultsStr[i] = results[i + 2].str();
            }
            else
            {
                std::wregex quoteRegex(L"^[>| ]*(.*)"); // 引用符の  > >  などがあった場合に無視してURLかを確認する
                if (!std::regex_search(orginalUrl_, results, quoteRegex))
                {
                    return false;
                }
                const std::wstring notQuoteUrl = results[1].str();
                if (!std::regex_search(notQuoteUrl, results, regex))
                {
                    return false;
                }
                for (int i = 0; i < 4; ++i)
                    resultsStr[i] = results[i + 2].str();
            }

            // 2017/08/27 memo
            // 下記のfor文の代わりのその下のresultsを直接osに代入するコードだとresultsが変な値になってて動かない。
            // release版だとOK。上の引用符対応のブロックを抜けるときにresultsの保持するメモリを壊しているように見える。
            // 別のresultsにコピーしてもnewで確保しても同じ挙動。
            // とりあえず、上記の通り、wstringにコピーして回避。
            // OSS対応中に、全く同じコードで問題なかったのが突然動かなくなった。
            // 差分を確認したところ変えたところはリソースが参照するWTLのバージョンくらい。
            std::wstringstream os;
            for (int i = 0; i < 4; ++i)
                os << resultsStr[i];
            //os << results[2].str() << results[3].str() << results[4].str() << results[5].str();


            urlExceptScheme_ = os.str();

            validSchemes_ |= scheme;
            validSchemes_ &= schemeFilter_;

            return true;
        }


        // C:やD:\など
        bool URL::initForFileUriLocalpath()
        {
            std::wregex regex(L"^<?([fF][iI][lL][eE]:[\\\\/]*)?([A-Za-z]:)([^\"|:*<>?]*)>?$");
            std::wsmatch results;
            if (!std::regex_search(orginalUrl_, results, regex))
            {
                std::wregex quoteRegex(L"^[>| ]*<?([fF][iI][lL][eE]:[\\\\/]*)?([A-Za-z]:)([^\"|:*<>?]*)>?$");
                if (!std::regex_search(orginalUrl_, results, quoteRegex))
                {
                    return false;
                }
            }

            // ドライブ文字の後の文字列は、上記の正規表現一つでは判定できなかったため分割
            std::wstring path = results[3].str();
            if (!path.empty())
            {
                // ドライブ文字の直後に :\ か :/ がくるか確認
                const wchar_t front = path.at(0);
                if ((L'\\' != front) && (L'/' != front))
                {
                    return false;
                }
            }

            // ドライブ文字の後の文字列に \\, \/, /\, //, のように2つ連続に\や/が続いている場合は\ひとつに置換
            std::wregex regexPath(L"[\\\\/]{2,}");
            path = std::regex_replace(path, regexPath, L"\\");


            std::wostringstream os;
            os << results[2].str() << path;
            urlExceptScheme_ = os.str();


            validSchemes_ |= Scheme::FILE_URI_LOCALPATH;
            validSchemes_ &= schemeFilter_;

            return true;
        }


        // \\で始まる文字列
        bool URL::initForFileUriUnc()
        {
            // 例えば \\abc.jp\path   \\localhost\path   \\127.0.0.0\path   \\computername\path
            std::wregex regex(
                L"^<?([fF][iI][lL][eE]:)?[\\\\/]{2,}([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)([^\"|:*<>?]*)>?$");
            std::wsmatch results;
            if (!std::regex_search(orginalUrl_, results, regex))
            {
                std::wregex quoteRegex(
                    L"^[>| ]*<?([fF][iI][lL][eE]:)?[\\\\/]{2,}([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)([^\"|:*<>?]*)>?$");
                if (!std::regex_search(orginalUrl_, results, quoteRegex))
                {
                    return false;
                }
            }

            // domain文字列の後の文字列は、上記の正規表現一つでは判定できなかったため分割
            std::wstring path = results[3].str();
            if (!path.empty())
            {
                // domain文字列の直後に \ か / がくるか確認
                const wchar_t front = path.at(0);
                if ((L'\\' != front) && (L'/' != front))
                {
                    return false;
                }
            }

            // domain文字列の後の文字列に \\, \/, /\, //, のように2つ連続に\や/が続いている場合は\ひとつに置換
            std::wregex regexPath(L"[\\\\/]{2,}");
            path = std::regex_replace(path, regexPath, L"\\");


            // validUncがどこかでtrueになればOK
            bool validUnc = false;
            const std::wstring domain = results[2].str();


            // domainがlocalhostならOK
            //if (!validUnc)
            //{
            std::wregex regexLocalhost(L"^[lL][oO][cC][aA][lL][hH][oO][sS][tT]$");
            if (std::regex_match(domain, regexLocalhost))
            {
                validUnc = true;
            }
            //}

            // domainがipならOK
            if (!validUnc)
            {
                std::wregex regexIp(L"^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$");
                std::wsmatch resultsIp;
                if (std::regex_search(domain, resultsIp, regexIp))
                {
                    // 0-255の範囲かチェック
                    bool invalidPeriod_0_255 = false;
                    for (int i = 1; i < 5; ++i)
                    {
                        if (!is0_255range(resultsIp[i].str()))
                        {
                            invalidPeriod_0_255 = true;
                            break;
                        }
                    }
                    if (!invalidPeriod_0_255)
                    {
                        validUnc = true;
                    }
                }
            }

            // toplevelDomainならOK
            if (!validUnc)
            {
                std::wstring lowerDomain(domain);
                StringUtils::to_lower(lowerDomain);
                std::wregex regexTopLevelDomain(
                    L"^([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?]+)\\.(com|net|org|edu|gov|mil|int|info|biz|name|pro|museum|aero|coop|jobs|travel|mobi|cat|asia|tel|post|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|ax|az|ba|bb|bd|be|bf|bg|bh|bi|bj|bl|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cu|cv|cx|cy|cz|de|dj|dk|dm|do|dz|ec|ee|eg|eh|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mf|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|rs|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|st|sv|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|um|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|xxx|ye|yt|za|zm|zw)$");
                if (std::regex_match(lowerDomain, regexTopLevelDomain))
                {
                    validUnc = true;
                }
            }

            // domainがcomputernameならOK
            if (!validUnc)
            {
                std::wregex regexComputername(L"^[a-zA-Z0-9\\-]+$");
                if (std::regex_match(domain, regexComputername))
                {
                    std::wregex regexNgComputerName(L"^[0-9]+$");
                    if (!std::regex_match(domain, regexNgComputerName))
                    {
                        validUnc = true;
                    }
                }
            }

            if (!validUnc)
            {
                return false;
            }

            std::wostringstream os;
            os << L"\\\\" << results[2].str() << path << results[4].str();
            urlExceptScheme_ = os.str();

            validSchemes_ |= Scheme::FILE_URI_UNC;
            validSchemes_ &= schemeFilter_;

            return true;
        }

        // helper
        std::wstring URL::validUrlExceptSchemeWithCut(const std::wstring& urlExceptScheme, bool* truncate)
        {
            if (truncate == nullptr)
            {
                return urlExceptScheme;
            }

            const std::wstring originalUrlExceptScheme(urlExceptScheme);
            std::experimental::filesystem::path url(urlExceptScheme);
            bool open = false;
            bool computerNameOnly = false;
            do
            {
                try
                {
                    open = std::experimental::filesystem::exists(url);
                    if (open)
                    {
                        DWORD dwSizeNeeded = 0;
                        ::GetFileSecurityW(url.wstring().c_str(),
                            DACL_SECURITY_INFORMATION,
                            nullptr,
                            0,
                            &dwSizeNeeded);
                        if (dwSizeNeeded != 0)
                        {
                            break;
                        }
                    }
                    std::wregex regex(L"^[\\\\/]{2}[^\\\\/]+$"); // \\で始まるPATHでその後に\や/がない場合（computer nameだけの場合）
                    if (std::regex_match(url.wstring(), regex))
                    {
                        // \\の後がcomputer nameだけの場合はexistsで必ずfalseになってしまう
                        // これ以上判定できないためここではopen=trueにしてとりあえずループ抜ける
                        // explorerで開ける場合も開けない場合もある
                        open = true;
                        std::wstring urlYEN = url.wstring();
                        urlYEN.replace(0, 2, L"\\\\");
                        urlYEN.append(L"\\");
                        if (originalUrlExceptScheme == urlYEN)
                        {
                            computerNameOnly = true;
                        }
                        break;
                    }
                }
                catch (...)
                {
                    open = false;
                }

                // truncate
                url = url.parent_path();
            } while (!url.empty());

            std::wstring validUrlExceptScheme = urlExceptScheme;
            if (open)
            {
                validUrlExceptScheme = url.wstring();
                std::wregex slashRegex(L"\\/");
                validUrlExceptScheme = std::regex_replace(validUrlExceptScheme, slashRegex, L"\\");
            }

            const std::experimental::filesystem::path original(originalUrlExceptScheme);
            const std::experimental::filesystem::path valid(validUrlExceptScheme);
            // 短縮形で開ける場合はcutはtrue。（\\computer_name\が\\computer_nameに\だけ除去されて短縮された場合は除く⇒cutはfalseにする。）
            *truncate = open && !computerNameOnly && (original.string() != valid.string());

            return validUrlExceptScheme;
        }


        // 192.168.0.1
        bool URL::initForIp()
        {
            // ipは最後の方にチェックするためここではscheme文字列がないものしかこない前提
            std::wregex regex(
                L"^<?([^#\\\\/]*@)?(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})(:[0-9]+)?([\\\\/].*)?>?$");
            std::wsmatch results;
            if (!std::regex_search(orginalUrl_, results, regex))
            {
                return false;
            }

            // 0-255の範囲かチェック
            for (int i = 2; i < 6; ++i)
            {
                if (!is0_255range(results[i].str()))
                {
                    return false;
                }
            }


            std::wostringstream os;
            os << results[1].str() << results[2].str() << L"." << results[3].str() << L"."
                << results[4].str() << L"." << results[5].str() << results[6].str() << results[7].str();
            urlExceptScheme_ = os.str();


            const std::wstring idPass = results[1].str();
            const std::wstring path = results[7].str();
            if (idPass.empty())
            {
                if (path.empty() || (L"\\" == path) || (L"/" == path))
                {
                    //ip and portだけ
                    validSchemes_ |= Scheme::FILE_URI_UNC | Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY | Scheme::TELNET;
                }
                else
                {
                    std::wregex regexPath(L"^[\\\\/][^\"|:*<>?]*$");
                    if (std::regex_match(path, regexPath))
                    {
                        validSchemes_ |= Scheme::FILE_URI_UNC;
                    }
                }
            }

            validSchemes_ |= Scheme::HTTP | Scheme::HTTPS | Scheme::FTP
                | Scheme::FTPS | Scheme::SFTP | Scheme::SVN | Scheme::SCP
                | Scheme::RCP;
            validSchemes_ &= schemeFilter_;

            return true;
        }


        // helper
        bool URL::is0_255range(const std::wstring& numStr)
        {
            int num = std::stoi(numStr);
            if ((0 <= num) && (num <= 255))
            {
                return true;
            }
            return false;
        }


        // localhost
        bool URL::initForLocalhost()
        {
            // localhostは最後の方にチェックするためここではscheme文字列がないものしかこない前提
            std::wregex regex(L"^<?([lL][oO][cC][aA][lL][hH][oO][sS][tT])(:[0-9]+)?([\\\\/].*)?>?$");
            std::wsmatch results;
            if (!std::regex_search(orginalUrl_, results, regex))
            {
                return false;
            }


            std::wostringstream os;
            os << results[1].str() << results[2].str() << results[3].str();
            urlExceptScheme_ = os.str();

            const std::wstring path = results[3].str();
            if (path.empty() || (L"\\" == path) || (L"/" == path))
            {
                validSchemes_ |= Scheme::FILE_URI_UNC | Scheme::TELNET;
            }
            else
            {
                std::wregex regexPath(L"^[\\\\/][^\"|:*<>?]*$");
                if (std::regex_match(path, regexPath))
                {
                    validSchemes_ |= Scheme::FILE_URI_UNC;
                }
            }

            validSchemes_ |= Scheme::HTTP | Scheme::HTTPS | Scheme::FTP
                | Scheme::FTPS | Scheme::SFTP | Scheme::SVN | Scheme::SCP
                | Scheme::RCP;
            validSchemes_ &= schemeFilter_;

            return true;
        }

        bool URL::simpleCheckScheme(const std::wstring& url)
        {
            // schemeがある場合はすでに対処ずみということでスキップ     inputUrl("HTTP://hoge.jp:1234/abc\"\"\"");がここに到達しちゃうのをスキップするため
            // todo 各関数でチェックした方がよいか、ここで一箇所で例外系を対処した方がよいか ⇒ まずは一箇所で対処2011/02/12
            std::wregex rExistsScheme(
                L"^([hH][tT]|[tT][pP][sS]?://|[fF][tT][pP][sS]?://|[tT][eE][lL][nN][eE][tT]://|[sS][vV][nN]://|[sS][cC][pP]://|[rR][cC][pP]://|[fF][iI][lL][eE]:|[\\\\/]{2}[^\\\\/]|(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})|[lL][oO][cC][aA][lL][hH][oO][sS][tT]).*");
            if (std::regex_match(url, rExistsScheme))
            {
                return true;
            }
            return false;
        }


        // top level domain (aaa.bbb.jp)
        bool URL::initForTopLevelDomain()
        {
            if (simpleCheckScheme(orginalUrl_))
            {
                return false;
            }

            std::wregex regex(
                L"^<?([^#\\\\/]*@)?([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?]+)(:[0-9]+)?([\\\\/].*)?>?$");
            std::wsmatch results;
            if (!std::regex_search(orginalUrl_, results, regex))
            {
                return false;
            }


            // top level domainの前の部分 ([^#]*:[^#\\\\/]*@)? のチェックはやめた  //std::wstring preDomain = results[1].str();


            // top level domain部のチェック
            std::wstring topLevelDomain = results[2].str();
            StringUtils::to_lower(topLevelDomain);
            std::wregex regexTopLevelDomain(
                L"^([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?]+)\\.(aero|arpa|asia|biz|cat|com|coop|edu|gov|info|int|jobs|mil|mobi|museum|name|net|org|pro|rich|tel|travel|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|ax|az|ba|bb|bd|be|bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cs|cu|cv|cx|cy|cz|dd|de|dj|dk|dm|do|dz|ec|ee|eg|eh|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|rs|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|st|su|sv|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|um|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zr|zw)$");
            std::wsmatch results_topLevelDomain;
            if (!std::regex_search(topLevelDomain, results_topLevelDomain, regexTopLevelDomain))
            {
                return false;
            }


            // domainだけかチェック
            const std::wstring idPass = results[1].str();
            const std::wstring path = results[4].str();
            if ((path.empty() || (L"\\" == path) || (L"/" == path)))
            {
                validSchemes_ |= Scheme::TELNET;
                if (idPass.empty())
                {
                    validSchemes_ |= Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY;
                }
            }


            std::wostringstream os;
            os << results[1].str() << results[2].str() << results[3].str() << results[4].str();
            urlExceptScheme_ = os.str();

            validSchemes_ |= Scheme::HTTP | Scheme::HTTPS | Scheme::FTP
                | Scheme::FTPS | Scheme::SFTP | Scheme::FILE_URI_UNC | Scheme::SVN
                | Scheme::SCP | Scheme::RCP;
            validSchemes_ &= schemeFilter_;

            return true;
        }


        // abc/a/s/c/s/
        bool URL::initForPlainpath()
        {
            if (simpleCheckScheme(orginalUrl_))
            {
                return false;
            }

            // std::wregex regex(L"^([^@!\"#$%&'\\(\\)=\\^~|`\\[\\{;\\+:*\\]\\},<>?\\\\/]+)([\\\\/].*)"); //←だと最初の/か\までの文字列に日本語が通ってしまう問題あり
            std::wregex regex(L"^([0-9a-zA-Z!\\-_\\.]+)([\\\\/].*)"); // 最初は/か\までの文字列に2byte文字（日本語）は受け付けない
            std::wsmatch results;
            if (!std::regex_search(orginalUrl_, results, regex))
            {
                return false;
            }

            urlExceptScheme_ = orginalUrl_;

            validSchemes_ |= Scheme::HTTP | Scheme::HTTPS | Scheme::FTP
                | Scheme::FTPS | Scheme::SFTP | Scheme::FILE_URI_UNC | Scheme::SVN
                | Scheme::SCP | Scheme::RCP;
            validSchemes_ &= schemeFilter_;

            return true;
        }

        // CR-Z とか computer name
        bool URL::initForOnlyComputername()
        {
            std::wregex regex(L"^([a-zA-Z0-9\\-]+)$");
            if (!std::regex_match(orginalUrl_, regex))
            {
                return false;
            }

            std::wregex rOnlyNum(L"^([0-9]+)$");
            if (std::regex_match(orginalUrl_, rOnlyNum))
            {
                return false;
            }

            urlExceptScheme_ = orginalUrl_;

            validSchemes_ |= Scheme::IP_OR_DOMAIN_OR_COMUTERNAME_ONLY;
            validSchemes_ &= schemeFilter_;

            return true;
        }

        // mail
        bool URL::initForMail()
        {
            std::wregex regex(L"^[a-zA-Z0-9\\._\\-]+@[a-zA-Z0-9\\._\\-]+$");
            if (!std::regex_match(orginalUrl_, regex))
            {
                return false;
            }

            urlExceptScheme_ = orginalUrl_;

            validSchemes_ |= Scheme::MAIL | Scheme::HTTP | Scheme::HTTPS
                | Scheme::FTP | Scheme::FTPS | Scheme::SFTP | Scheme::TELNET
                | Scheme::SVN | Scheme::SCP | Scheme::RCP;
            validSchemes_ &= schemeFilter_;

            return true;
        }

    }
}
