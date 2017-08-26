/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "RegexSearcher.h"

#include "bkzoo_log.h"

#include <sstream>
#include <regex>

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace util
    {

        /// @todo (yoichi) 2016/08/21 このクラスよくわからない。リファクタ必要。
        // wordまたはregexに問題がある場合はエラーにしない方針。
        // エラーにするのは明確にヒットしない場合だけ。
        // エラーの場合はisValid_はfalseでactualSearchWord_は空文字
        RegexSearcher::RegexSearcher(const std::wstring& word, const std::wstring& regexString)
            : isValid_(true), actualSearchWord_(word)
        {
            if (word.empty())
            {
                return;
            }
            if (regexString.empty())
            {
                return;
            }

            if (!RegexUtil::isValidRegex(regexString))
            {
                // 文字列が正規表現になってない
                isValid_ = false;
                return;
            }

            std::wregex regex(regexString);
            std::wsmatch results;
            if (!std::regex_search(word, results, regex))
            {
                isValid_ = false;
                actualSearchWord_ = L"";
                return;
            }
            if (results.size() < 2)
            {
                return;
            }
            if (!results.str(1).empty())
            {
                actualSearchWord_ = results.str(1);
            }
        }

        bool RegexSearcher::validate() const
        {
            return isValid_;
        }

        std::wstring RegexSearcher::getActualSearchWord() const
        {
            return actualSearchWord_;
        }

        bool RegexUtil::isValidRegex(const std::wstring& regexString)
        {
            std::wregex regex;
            try
            {
                regex.assign(regexString);
            }
            catch (const std::exception& /*e*/)
            {
                return false;
            }
            return true;
        }

    }
}
