/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "SearchWord.h"

#include "command/helper/SearchWordHelper.h"
#include "util/RegexSearcher.h"
#include "util/StringUtil.h"
#include "util/MessageBox.h"

#include "bkzoo_string.h"

#include "resource.h"

#include <BeckyAPI.h>

#include <sstream>
#include <fstream>
#include <regex>

#include "debug/detect_memory_leak.h"

extern CBeckyAPI beckyApi; // You can have only one instance in a project.

namespace bkzoo
{
    namespace command
    {

        SearchWordCommand::SearchWordCommand(const SearchWordCommand::Param& param)
            : param_(param)
        {
        }

        bool SearchWordCommand::execute()
        {
            // 正規表現で検索キーワードを抜き出し (例えば #11 => 11)
            util::RegexSearcher regexSearcher(param_.selectedString, param_.regexpString);

            std::string searchUrl;

            // POST method の処理
            if (param_.method == config::MethodType::POST)
            {
                // URLにencodeした検索キーワード埋め込む
                const std::wregex placeholderRegex(L"\\{\\}");
                std::wstring searchUrl_w = std::regex_replace(param_.url, placeholderRegex, regexSearcher.getActualSearchWord());

                searchUrl = generatePostHtmlFile(searchUrl_w, param_.encode);
            }
            // GET method の処理
            else
            {
                const std::string actualSearchWord = wm_cast<std::string>(regexSearcher.getActualSearchWord(), param_.encode);
                //urlEncodeにより検索キーワードをエンコード
                const std::string encodedSearchWord = param_.urlEncode(wm_cast<std::string>(actualSearchWord, param_.encode));

                //URLにencodeした検索キーワード埋め込む
                const std::regex placeholderRegex("\\{\\}");
                searchUrl = std::regex_replace(wm_cast<std::string>(param_.url, param_.encode), placeholderRegex, encodedSearchWord);
            }

            // 警告表示
            if (!util::okCancelWarningMessageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_WARNING_OPEN_URL))
            {
                // skip
                return true;
            }

            //ブラウザで開く
            ::ShellExecuteA(param_.hWnd, "open", searchUrl.c_str(), nullptr, nullptr, SW_SHOW);

            return true;
        }

        std::string SearchWordCommand::generatePostHtmlFile(const std::wstring& searchUrl, int encode)
        {
            std::ostringstream postHtmlFileName;
#ifdef DATA_FOLDER
            LPCSTR folder = (LPCSTR)::beckyApi.GetDataFolder();
            postHtmlFile << folder << "PlugIns\\BkZOO2" << sjis_SiteNameStr << getTimeStr() << ".html";
#else
            LPCSTR folder = (LPCSTR)::beckyApi.GetTempFolder();
            postHtmlFileName << folder << "BkZOO2" << util::stringFromTime() << ".html";
#endif
            helper::PostParam postParam = helper::getPostParam(searchUrl);

            std::wstring charset = L"utf-8";
            switch (encode)
            {
            case CP_UTF8:
                charset = L"utf-8";
                break;
            case CP_ACP:
                charset = L"Shift_JIS";
                break;
            case CP51932:
                charset = L"EUC-JP";
                break;
            default:
                // do nothing
                break;
            }

            std::wostringstream html;
            html << L"<!DOCTYPE html>\n";
            html << L"<html>\n";
            html << L"<head>\n";
            html << L"<meta charset=\"" << charset << "\">\n";
            html << L"</head>\n";
            html << L"<body onload=\"document.forms[0].submit()\">\n";
            html << L"<form action=\"" << postParam.actionUrl << L"\" method=\"post\">\n";

            for (auto& kv : postParam.params)
            {
                html << L"<input type=\"hidden\" name=\"" << kv.first;
                html << L"\" value=\"" << kv.second << L"\"/>\n";
            }

            html << L"</form>\n";
            html << L"</body>\n";
            html << L"</html>\n";

            std::ofstream htmlFile(postHtmlFileName.str().c_str());
            htmlFile << wm_cast<std::string>(html.str(), encode);

            return postHtmlFileName.str();
        }

    }
}
