/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_CONFIG_CONFIG_H
#define BKZ_CONFIG_CONFIG_H

#include <Windows.h>

#include <string>
#include <vector>
#include <map>
#include <list>
#include <memory>

#include "defs.h"

class ConfigTest;
namespace bkzoo
{
    namespace config
    {

        /// ここで割り当てた値はデフォルト値
        enum class App : wchar_t
        {
            Browzer = L'1',
            Explorer = L'3',
            FFFTP = L'T',
            FileZilla = L'Z',
            Google = L'G',
            PasteBracket = L'K',
            RemoteDesktop = L'R',
            TeraTerm = L'M',
            TortoiseSVN = L'V',
            WinSCP = L'W',
        };

        class General final
        {
        public:
            General() = default;
            ~General() = default;

            wchar_t shortcut(App app) const;
            void setShortcut(App app, const std::wstring& shortcut);

            const std::wstring& tripleClick() const;
            bool tripleClick_bool() const;
            void setTripleClick(const std::wstring& tripleClick);

            const std::wstring& ctrlDoubleClick() const;
            bool ctrlDoubleClick_bool() const;
            void setCtrlDoubleClick(const std::wstring& ctrlDoubleClick);

            const std::wstring& ctrlTripleClick() const;
            bool ctrlTripleClick_bool() const;
            void setCtrlTripleClick(const std::wstring& ctrlTripleClick);

            const std::wstring& version() const;
            void setVersion(const std::wstring& version);

            const std::wstring& checkBeckyUpdate() const;
            bool checkBeckyUpdate_bool() const;
            void setCheckBeckyUpdate(const std::wstring& checkBeckyUpdate);

            const std::wstring& warningOpenUrl() const;
            bool warningOpenUrl_bool() const;
            void setWarningOpenUrl(const std::wstring& warningOpenUrl);

            // for test
            //bool operator==(const General& lhs) const;

        private:
            std::map<const App, wchar_t> shortcuts_;
            std::wstring tripleClick_;
            std::wstring ctrlDoubleClick_;
            std::wstring ctrlTripleClick_;
            std::wstring version_;
            std::wstring checkBeckyUpdate_;
            std::wstring warningOpenUrl_;
        };

        enum class MethodType : int
        {
            GET,
            POST,
        };

        class Site final
        {
        public:
            Site() = default;
            ~Site() = default;

            const std::wstring& clickable() const;
            bool clickable_bool() const;
            void setClickable(const std::wstring& clickable);

            const std::wstring& enabled() const;
            bool enabled_bool() const;
            void setEnabled(const std::wstring& enabled);

            const std::wstring& encode() const;
            int encode_int() const;
            void setEncode(const std::wstring& encode);

            const std::wstring& method() const;
            MethodType method_enum() const;
            void setMethod(const std::wstring& method);

            const std::wstring& preset() const;
            bool preset_bool() const;
            void setPreset(const std::wstring& preset);

            const std::wstring& presetid() const;
            int presetid_int() const;
            void setPresetid(const std::wstring& presetid);

            const std::wstring& regex() const;
            void setRegex(const std::wstring& regex);

            const std::wstring& shortcut() const;
            wchar_t shortcut_char() const;
            void setShortcut(const std::wstring& shortcut);

            const std::wstring& title() const;
            void setTitle(const std::wstring& title);

            const std::wstring& url() const;
            void setUrl(const std::wstring& url);

            std::wstring statusbar() const;
            //            void setStatusbar(const std::wstring& statusbar); ///< @todo (yoichi) 使ってない

        private:
            std::wstring clickable_;
            std::wstring enabled_;
            std::wstring encode_;
            std::wstring method_;
            std::wstring preset_;
            std::wstring presetid_;
            std::wstring customid_;
            std::wstring regex_;
            std::wstring shortcut_;
            std::wstring statusbar_;
            std::wstring title_;
            std::wstring url_;
        };

        typedef std::map<int, Site> SiteMap;
        typedef std::vector<Site> SiteVector;
        typedef std::list<Site> SiteList;

        typedef std::map<const UINT, const Site&> MenuCommandIdSiteMap;

        class Config final
        {
            friend class ConfigService;
            friend class ConfigFactory;
            friend struct std::default_delete<Config>; // for singleton pattern with unique_ptr 
            friend class ConfigTest;

        public:
            static Config& instance();

        private:
            static void setInstance(std::unique_ptr<Config> config);

        public:
            const General& general() const;
            void setGeneral(const General& general);

            /// default値は '\0'
            wchar_t shortcut(App app) const;

            // Version
            const std::wstring& version() const;
            void setVersion(const std::wstring& version);
            bool checkBeckyUpdate() const;
            void setCheckBeckyUpdate(bool checkBeckyUpdate);

            // Mouse
            bool tripleClick() const;
            void setTripleClick(bool tripleClick);
            bool ctrlDoubleClick() const;
            void setCtrlDoubleClick(bool doubleClick);
            bool ctrlTripleClick() const;
            void setCtrlTripleClick(bool doubleClick);
            bool warningOpenUrl() const;
            void setWarningOpenUrl(bool warningOpenUrl);

            // Sites
            const SiteList& sites() const;
            void setSites(const SiteList& sites);

            // menuCommandIdSiteMap
            const MenuCommandIdSiteMap& menuCommandIdSiteMap() const;
            void setMenuCommandIdSiteMap(const MenuCommandIdSiteMap& menuCommandIdSiteMap);

        private:
            static std::unique_ptr<Config> instance_;

            General general_;
            SiteList sites_;

            MenuCommandIdSiteMap menuCommandIdSiteMap_;

            Config() = default;
            ~Config() = default;

            DISALLOW_COPY_AND_ASSIGN(Config);
        };

    }
}

#endif // BKZ_CONFIG_CONFIG_H
