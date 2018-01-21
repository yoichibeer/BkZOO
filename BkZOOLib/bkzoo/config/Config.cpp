/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Config.h"

#include "util/PathUtils.h"
#include "bkzoo_string.h"

#include <sstream> // statusbar()
#include <regex> // for setShortcut()

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace config
    {

        const SiteList& Config::sites() const
        {
            return sites_;
        }
        void Config::setSites(const SiteList& sites)
        {
            sites_ = sites;
        }


        const MenuCommandIdSiteMap& Config::menuCommandIdSiteMap() const
        {
            return menuCommandIdSiteMap_;
        }
        void Config::setMenuCommandIdSiteMap(const MenuCommandIdSiteMap& menuCommandIdSiteMap)
        {
            menuCommandIdSiteMap_ = menuCommandIdSiteMap;
        }


        std::unique_ptr<Config> Config::instance_;

        Config& Config::instance()
        {
            assert((instance_.get() != nullptr) && "Call setInscance() first!");
            return *instance_;
        }
        void Config::setInstance(std::unique_ptr<Config> config)
        {
            instance_.reset(config.release());
        }


        const General& Config::general() const
        {
            return general_;
        }
        void Config::setGeneral(const General & general)
        {
            general_ = general;
        }


        wchar_t Config::shortcut(App app) const
        {
            return general_.shortcut(app);
        }


        const std::wstring& Config::version() const
        {
            return general_.version();
        }
        void Config::setVersion(const std::wstring& version)
        {
            general_.setVersion(version);
        }


        bool Config::checkBeckyUpdate() const
        {
            return general_.checkBeckyUpdate_bool();
        }
        void Config::setCheckBeckyUpdate(bool checkBeckyUpdate)
        {
            general_.setCheckBeckyUpdate(checkBeckyUpdate ? L"true" : L"false");
        }


        bool Config::tripleClick() const
        {
            return general_.tripleClick_bool();
        }
        void Config::setTripleClick(bool tripleClick)
        {
            general_.setTripleClick(tripleClick ? L"true" : L"false");
        }


        bool Config::ctrlDoubleClick() const
        {
            return general_.ctrlDoubleClick_bool();
        }
        void Config::setCtrlDoubleClick(bool doubleClick)
        {
            general_.setCtrlDoubleClick(doubleClick ? L"true" : L"false");
        }


        bool Config::ctrlTripleClick() const
        {
            return general_.ctrlTripleClick_bool();
        }
        void Config::setCtrlTripleClick(bool tripleClick)
        {
            general_.setCtrlTripleClick(tripleClick ? L"true" : L"false");
        }


        bool Config::warningOpenUrl() const
        {
            return general_.warningOpenUrl_bool();
        }
        void Config::setWarningOpenUrl(bool warningOpenUrl)
        {
            general_.setWarningOpenUrl(warningOpenUrl ? L"true" : L"false");
        }


        log::LogLevel Config::logLevel() const
        {
            return general_.logLevel_enum();
        }
        void Config::setLogLevel(log::LogLevel logLevel)
        {
            std::wstring logLevelStr;
            switch (logLevel)
            {
            case log::LogLevel::Non:
                logLevelStr = L"0";
                break;
            case log::LogLevel::Fatal:
                logLevelStr = L"1";
                break;
            case log::LogLevel::Error:
                logLevelStr = L"2";
                break;
            case log::LogLevel::Warning:
                logLevelStr = L"3";
                break;
            case log::LogLevel::Info:
                logLevelStr = L"4";
                break;
            case log::LogLevel::Debug:
                logLevelStr = L"5";
                break;
            case log::LogLevel::Trace:
                logLevelStr = L"6";
                break;
            default:
                assert(false);
                // do nothing
            }
            general_.setLogLevel(logLevelStr);
        }


        const std::wstring& Site::clickable() const
        {
            return clickable_;
        }
        bool Site::clickable_bool() const
        {
            return (clickable_ == L"true"); // default value は false
        }
        void Site::setClickable(const std::wstring& clickable)
        {
            clickable_ = clickable;
        }


        const std::wstring& Site::enabled() const
        {
            return enabled_;
        }
        bool Site::enabled_bool() const
        {
            if (enabled_.empty())
                return true; // default value
            return (enabled_ == L"true");
        }
        void Site::setEnabled(const std::wstring& enabled)
        {
            enabled_ = enabled;
        }


        const std::wstring& Site::encode() const
        {
            return encode_;
        }
        int Site::encode_int() const
        {
            if (encode_ == L"UTF-8")
                return CP_UTF8;

            if (encode_ == L"Shift_JIS")
                return CP_ACP;

            if (encode_ == L"EUC-JP")
                return CP51932;

            return CP_ACP;
        }
        void Site::setEncode(const std::wstring& encode)
        {
            encode_ = encode;
        }


        const std::wstring& Site::method() const
        {
            return method_;
        }
        MethodType Site::method_enum() const
        {
            if (method_ == L"POST")
                return MethodType::POST;

            if (encode_ == L"GET")
                return MethodType::GET;

            return MethodType::GET;
        }
        void Site::setMethod(const std::wstring& method)
        {
            method_ = method;
        }


        const std::wstring& Site::preset() const
        {
            return preset_;
        }
        bool Site::preset_bool() const
        {
            return (preset_ == L"true");
        }
        void Site::setPreset(const std::wstring& preset)
        {
            preset_ = preset;
        }


        const std::wstring& Site::presetid() const
        {
            assert(!presetid_.empty());
            return presetid_;
        }
        int Site::presetid_int() const
        {
            assert(StringUtils::isNumber(presetid_));
            return std::stoi(presetid_.c_str());
        }
        void Site::setPresetid(const std::wstring& presetid)
        {
            assert(StringUtils::isNumber(presetid));
            presetid_ = presetid;
        }


        const std::wstring& Site::regex() const
        {
            return regex_;
        }
        void Site::setRegex(const std::wstring& regex)
        {
            regex_ = regex;
        }


        const std::wstring& Site::shortcut() const
        {
            return shortcut_;
        }
        wchar_t Site::shortcut_char() const
        {
            return shortcut_.empty() ? L'\0' : shortcut_.at(0);
        }
        void Site::setShortcut(const std::wstring& shortcut)
        {
            shortcut_ = shortcut.empty() ? shortcut : shortcut.substr(0, 1);
        }


        std::wstring Site::statusbar() const
        {
            if (!statusbar_.empty())
                return statusbar_;

            assert(statusbar_.empty());

            if (title_.empty())
                return L"";

            std::wostringstream statusbar;
            statusbar << L"選択文字列を" << title_; /// @todo (yoichi) i18nはどうしよう？
            return statusbar.str();
        }
        //void Site::setStatusbar(const std::wstring& statusbar)
        //{
        //    statusbar_ = statusbar;
        //}


        const std::wstring& Site::title() const
        {
            return title_;
        }
        void Site::setTitle(const std::wstring& title)
        {
            title_ = title;
        }


        const std::wstring& Site::url() const
        {
            return url_;
        }
        void Site::setUrl(const std::wstring& url)
        {
            url_ = url;
        }





        wchar_t General::shortcut(App app) const
        {
            const auto& it = shortcuts_.find(app);
            if (it != shortcuts_.end())
                return it->second;

            // default value
            // app自体の値がショートカットの文字でデフォルト値。
            return static_cast<wchar_t>(app);
        }
        void General::setShortcut(App app, const std::wstring& shortcut)
        {
            // 最初の文字が[0-9A-Z]の時だけそのまま使う
            std::wregex regexNumAndUpper(L"^([0-9A-Z]).*$");
            if (std::regex_match(shortcut, regexNumAndUpper))
            {
                shortcuts_[app] = shortcut.at(0);
                return;
            }

            // 小文字、記号、全角などはnull文字
            shortcuts_[app] = '\0';
        }


        const std::wstring& General::tripleClick() const
        {
            return tripleClick_;
        }
        bool General::tripleClick_bool() const
        {
            if (tripleClick_.empty())
                return true; // default value
            return  (tripleClick_ == L"true");
        }
        void General::setTripleClick(const std::wstring& tripleClick)
        {
            tripleClick_ = tripleClick;
        }


        const std::wstring& General::ctrlDoubleClick() const
        {
            return ctrlDoubleClick_;
        }
        bool General::ctrlDoubleClick_bool() const
        {
            if (ctrlDoubleClick_.empty())
                return true; // default value
            return  (ctrlDoubleClick_ == L"true");
        }
        void General::setCtrlDoubleClick(const std::wstring& ctrlDoubleClick)
        {
            ctrlDoubleClick_ = ctrlDoubleClick;
        }


        const std::wstring& General::ctrlTripleClick() const
        {
            return ctrlTripleClick_;
        }
        bool General::ctrlTripleClick_bool() const
        {
            if (ctrlTripleClick_.empty())
                return true; // default value
            return  (ctrlTripleClick_ == L"true");
        }
        void General::setCtrlTripleClick(const std::wstring& ctrlTripleClick)
        {
            ctrlTripleClick_ = ctrlTripleClick;
        }


        const std::wstring& General::version() const
        {
            return version_;
        }
        void General::setVersion(const std::wstring& version)
        {
            version_ = version;
        }

        const std::wstring& General::checkBeckyUpdate() const
        {
            return checkBeckyUpdate_;
        }
        bool General::checkBeckyUpdate_bool() const
        {
            if (checkBeckyUpdate_.empty())
                return true; // default value
            return  (checkBeckyUpdate_ == L"true");
        }
        void General::setCheckBeckyUpdate(const std::wstring& checkBeckyUpdate)
        {
            checkBeckyUpdate_ = checkBeckyUpdate;
        }


        const std::wstring& General::warningOpenUrl() const
        {
            return warningOpenUrl_;
        }
        bool General::warningOpenUrl_bool() const
        {
            if (warningOpenUrl_.empty())
                return true; // default value
            return  (warningOpenUrl_ == L"true");
        }
        void General::setWarningOpenUrl(const std::wstring& warningOpenUrl)
        {
            warningOpenUrl_ = warningOpenUrl;
        }


        const std::wstring& General::logLevel() const
        {
            return logLevel_;
        }
        log::LogLevel General::logLevel_enum() const
        {
            if (logLevel_ == L"0")
                return log::LogLevel::Non;

            if (logLevel_ == L"1")
                return log::LogLevel::Fatal;

            if (logLevel_ == L"2")
                return log::LogLevel::Error;

            if (logLevel_ == L"3")
                return log::LogLevel::Warning;

            if (logLevel_ == L"4")
                return log::LogLevel::Info;

            if (logLevel_ == L"5")
                return log::LogLevel::Debug;

            if (logLevel_ == L"6")
                return log::LogLevel::Trace;

            return log::LogLevel::Error; // default value
        }
        void General::setLogLevel(const std::wstring& logLevel)
        {
            logLevel_ = logLevel;
        }


        // for test
        //bool General::operator==(const General& lhs) const
        //{
        //    if (lhs.ctrlDoubleClick_ != ctrlDoubleClick_)
        //        return false;

        //    if (lhs.shortcuts_ != shortcuts_)
        //        return false;

        //    if (lhs.tripleClick_ != tripleClick_)
        //        return false;

        //    if (lhs.version_ != version_)
        //        return false;

        //    return true;
        //}

    }
}
