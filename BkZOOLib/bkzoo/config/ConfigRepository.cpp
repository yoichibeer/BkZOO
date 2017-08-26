/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "ConfigRepository.h"

#include "ConfigFactory.h"

#include "bkzoo_string.h"

#include <filesystem>
#include <sstream>

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace config
    {

        // 無名namespaceで定数定義
        namespace
        {

            //
            // General用とSite用の共通定数
            // 

            const std::wstring VALUE_true(L"true");
            const std::wstring VALUE_false(L"false");

            const std::wstring APP_site(L"site");
            const IniUnit INIUNIT_counter = { APP_site, L"counter", L"" };
            const IniUnit INIUNIT_SITE_REMOVER = { APP_site, L"", L"" }; // for site remove


            //
            // General用の定数
            //

            const std::wstring APP_shortcut(L"shortcut");
            const std::map<App, IniUnit> INIUNIT_shortcuts =
            {
                { App::Browzer, { APP_shortcut, L"Browzer", L"1" } },
                { App::Explorer, { APP_shortcut, L"Explorer", L"3" } },
                { App::FFFTP, { APP_shortcut, L"FFFTP", L"T" } },
                { App::FileZilla, { APP_shortcut, L"FileZilla", L"Z" } },
                { App::Google, { APP_shortcut, L"Google", L"G" } },
                { App::PasteBracket, { APP_shortcut, L"PasteBracket", L"K" } },
                { App::RemoteDesktop, { APP_shortcut, L"RemoteDesktop", L"R" } },
                { App::TeraTerm, { APP_shortcut, L"TeraTerm", L"M" } },
                { App::TortoiseSVN, { APP_shortcut, L"TortoiseSVN", L"V" } },
                { App::WinSCP, { APP_shortcut, L"WinSCP", L"W" } },
            };

            const std::wstring APP_mouse(L"mouse");
            const IniUnit INIUNIT_tripleclick = { APP_mouse, L"tripleclick", VALUE_true };
            const IniUnit INIUNIT_ctrldoubleclick = { APP_mouse, L"ctrldoubleclick", VALUE_true };
            const IniUnit INIUNIT_ctrltripleclick = { APP_mouse, L"ctrltripleclick", VALUE_true };

            const std::wstring APP_general(L"general");
            const IniUnit INIUNIT_version = { APP_general, L"version", L"" };
            const IniUnit INIUNIT_checkBeckyUpdate = { APP_general, L"checkBeckyUpdate", L"" };
            const IniUnit INIUNIT_warningopenurl = { APP_general, L"warningopenurl", L"" };


            //
            // Site用の定数
            //

            const IniUnit INIUNIT_clickable = { APP_site, L"clickable", L"" };
            const IniUnit INIUNIT_enabled = { APP_site, L"enabled", L"" };
            const IniUnit INIUNIT_encode = { APP_site, L"encode", L"" };
            const IniUnit INIUNIT_presetid = { APP_site, L"presetid", L"" };
            const IniUnit INIUNIT_method = { APP_site, L"method", L"" };
            const IniUnit INIUNIT_preset = { APP_site, L"preset", L"" };
            const IniUnit INIUNIT_regex = { APP_site, L"regex", L"" };
            const IniUnit INIUNIT_shorcut = { APP_site, L"shortcut", L"" };
            const IniUnit INIUNIT_statusbar = { APP_site, L"statusbar", L"" };
            const IniUnit INIUNIT_title = { APP_site, L"title", L"" };
            const IniUnit INIUNIT_url = { APP_site, L"url", L"" };
        }



        ConfigRepository& ConfigRepository::instance()
        {
            static ConfigRepository configRepository;
            return configRepository;
        }

        bool ConfigRepository::initialize(const std::wstring& standardPath, const std::wstring& presetPath)
        {
            //            if (std::experimental::filesystem::exists(standardPath))
            standardRepository_.initialize(standardPath);

            //            if (std::experimental::filesystem::exists(presetPath))
            presetRepository_.initialize(presetPath);

            return true;
        }

        std::unique_ptr<Config> ConfigRepository::loadConfig() const
        {
            const General general = readGeneral();
            const SiteMap presetSites = readPresetSites();
            const SiteVector standardSites = readStandardSites();

            return ConfigFactory::create(general, presetSites, standardSites);
        }

        /// @todo (yoichi) 2016/10/03 設定値が一つだけ変わっててもすべて保存する。設定値一つだけ保存のオプションあってもいいかも。INIUNIT_tripleclickとかを指定するなど。
        bool ConfigRepository::saveConfig(const Config& config) const
        {
            writeGeneral(config.general());
            writeStandardSites(config.sites());

            return true;
        }



        General ConfigRepository::readGeneral() const
        {
            if (standardRepository_.iniPath().empty())
                return General();

            General general;
            {
                general.setTripleClick(standardRepository_.readValue(INIUNIT_tripleclick));
                general.setCtrlDoubleClick(standardRepository_.readValue(INIUNIT_ctrldoubleclick));
                general.setCtrlTripleClick(standardRepository_.readValue(INIUNIT_ctrltripleclick));
                general.setVersion(standardRepository_.readValue(INIUNIT_version));
                general.setCheckBeckyUpdate(standardRepository_.readValue(INIUNIT_checkBeckyUpdate));
                general.setWarningOpenUrl(standardRepository_.readValue(INIUNIT_warningopenurl));

                setShortcutToGeneral(&general, App::Browzer);
                setShortcutToGeneral(&general, App::Explorer);
                setShortcutToGeneral(&general, App::FFFTP);
                setShortcutToGeneral(&general, App::FileZilla);
                setShortcutToGeneral(&general, App::Google);
                setShortcutToGeneral(&general, App::PasteBracket);
                setShortcutToGeneral(&general, App::RemoteDesktop);
                setShortcutToGeneral(&general, App::TeraTerm);
                setShortcutToGeneral(&general, App::TortoiseSVN);
                setShortcutToGeneral(&general, App::WinSCP);
            }

            return general;
        }



        SiteMap ConfigRepository::readPresetSites() const
        {
            return presetRepository_.readPresetSites();
        }



        SiteVector ConfigRepository::readStandardSites() const
        {
            return standardRepository_.readStandardSites();
        }



        void ConfigRepository::writeGeneral(const General& general) const
        {
            writeShortcutToStandardIni(general, App::Browzer);
            writeShortcutToStandardIni(general, App::Explorer);
            writeShortcutToStandardIni(general, App::FFFTP);
            writeShortcutToStandardIni(general, App::FileZilla);
            writeShortcutToStandardIni(general, App::Google);
            writeShortcutToStandardIni(general, App::PasteBracket);
            writeShortcutToStandardIni(general, App::RemoteDesktop);
            writeShortcutToStandardIni(general, App::TeraTerm);
            writeShortcutToStandardIni(general, App::TortoiseSVN);
            writeShortcutToStandardIni(general, App::WinSCP);

            standardRepository_.writeValue(INIUNIT_tripleclick, general.tripleClick());
            standardRepository_.writeValue(INIUNIT_ctrldoubleclick, general.ctrlDoubleClick());
            standardRepository_.writeValue(INIUNIT_ctrltripleclick, general.ctrlTripleClick());
            standardRepository_.writeValue(INIUNIT_version, general.version());
            standardRepository_.writeValue(INIUNIT_checkBeckyUpdate, general.checkBeckyUpdate());
            standardRepository_.writeValue(INIUNIT_warningopenurl, general.warningOpenUrl());
        }

        void ConfigRepository::removeStandardSites() const
        {
            const std::wstring counterStr = standardRepository_.readValue(INIUNIT_counter);
            if (!StringUtils::isPositiveNumber(counterStr))
                return;

            const int counter = std::stoi(counterStr);
            for (int index = 0; index < counter; ++index)
            {
                standardRepository_.writeValue(INIUNIT_SITE_REMOVER, L"", index);
            }

            standardRepository_.writeValue(INIUNIT_counter, L"0");
        }



        void ConfigRepository::writeStandardSites(const SiteList& sites) const
        {
            removeStandardSites();

            int siteCounter = 0;
            for (const auto& site : sites)
            {
                if (site.preset_bool())
                    standardRepository_.writePresetSite(site, siteCounter++);
                else
                    standardRepository_.writeCustomSite(site, siteCounter++);
            }

            // site の個数
            standardRepository_.writeValue(INIUNIT_counter, std::to_wstring(siteCounter));
        }



        void ConfigRepository::setShortcutToGeneral(General* general, App app) const
        {
            assert(general != nullptr);
            general->setShortcut(app, standardRepository_.readValue(INIUNIT_shortcuts.at(app)));
        }



        void ConfigRepository::writeShortcutToStandardIni(const General& general, App app) const
        {
            standardRepository_.writeValue(INIUNIT_shortcuts.at(app), std::wstring(1, general.shortcut(app)));
        }



        std::wstring ConfigRepository::IniRepository::readValue(const IniUnit& iniUnit) const
        {
            wchar_t buffer[_MAX_PATH] = { L'\0' };
            ::GetPrivateProfileString(
                iniUnit.appName.c_str(),
                iniUnit.keyName.c_str(),
                iniUnit.defaultValue.c_str(),
                buffer,
                _MAX_PATH,
                iniPath().c_str()
            );

            return std::wstring(buffer);
        }



        std::wstring ConfigRepository::IniRepository::readValue(const IniUnit & iniUnit, int index) const
        {
            std::wostringstream appName;
            appName << iniUnit.appName << index;

            wchar_t buffer[_MAX_PATH] = { L'\0' };
            ::GetPrivateProfileString(
                appName.str().c_str(),
                iniUnit.keyName.c_str(),
                iniUnit.defaultValue.c_str(),
                buffer,
                _MAX_PATH,
                iniPath().c_str()
            );

            return std::wstring(buffer);
        }



        BOOL ConfigRepository::IniRepository::writeValue(const IniUnit& iniUnit, const std::wstring& value) const
        {
            return ::WritePrivateProfileString(
                iniUnit.appName.c_str(),
                iniUnit.keyName.c_str(),
                value.c_str(),
                iniPath().c_str()
            );
        }



        BOOL ConfigRepository::IniRepository::writeValue(const IniUnit& iniUnit, const std::wstring& value, int index) const
        {
            std::wostringstream appName;
            appName << iniUnit.appName << index;

            return ::WritePrivateProfileString(
                appName.str().c_str(),
                iniUnit.keyName.empty() ? nullptr/* for remove*/ : iniUnit.keyName.c_str(),
                value.empty() ? nullptr/* for remove*/ : value.c_str(),
                iniPath().c_str()
            );
        }



        SiteVector ConfigRepository::IniRepository::readStandardSites() const
        {
            if (iniPath().empty())
                return SiteVector();

            // site counter
            const std::wstring counterStr = readValue(INIUNIT_counter);
            if (!StringUtils::isNumber(counterStr))
                return SiteVector();

            const int counter = std::stoi(counterStr);

            // sites
            SiteVector sites;
            for (int index = 0; index < counter; ++index)
            {
                config::Site site;

                // preset と custom に共通
                site.setEnabled(readValue(INIUNIT_enabled, index));
                site.setPreset(readValue(INIUNIT_preset, index));
                if (site.preset_bool())
                {
                    // preset site
                    const std::wstring presetid = readValue(INIUNIT_presetid, index);
                    if (!StringUtils::isPositiveNumber(presetid))
                        // presetidはpreset siteには必須項目なのでなければスキップ
                        continue;

                    // presetidの重複は許す
                    site.setPresetid(presetid);
                }
                else
                {
                    // custom site
                    site.setTitle(readValue(INIUNIT_title, index));
                    site.setShortcut(readValue(INIUNIT_shorcut, index));
                    site.setUrl(readValue(INIUNIT_url, index));
                    site.setEncode(readValue(INIUNIT_encode, index));
                    site.setMethod(readValue(INIUNIT_method, index));
                    site.setRegex(readValue(INIUNIT_regex, index));
                    site.setClickable(readValue(INIUNIT_clickable, index));
                }

                // sitesに追加
                sites.push_back(site);
            }

            return sites;
        }

        SiteMap ConfigRepository::IniRepository::readPresetSites() const
        {
            if (iniPath().empty())
                return SiteMap();

            // site counter
            const std::wstring counterStr = readValue(INIUNIT_counter);
            if (!StringUtils::isNumber(counterStr))
                return SiteMap();

            const int counter = std::stoi(counterStr);

            // preset sites
            SiteMap sites;
            for (int index = 0; index < counter; ++index)
            {
                config::Site site;
                site.setPreset(readValue(INIUNIT_preset, index));
                if (!site.preset_bool())
                    continue; // preset site だけのはず

                // presetidの重複は許す
                const std::wstring presetid = readValue(INIUNIT_presetid, index);
                if (!StringUtils::isPositiveNumber(presetid))
                    continue; // presetid は preset site には必須項目なのでなければスキップ

                site.setPresetid(presetid);
                site.setEnabled(readValue(INIUNIT_enabled, index));

                site.setTitle(readValue(INIUNIT_title, index));
                site.setShortcut(readValue(INIUNIT_shorcut, index));
                site.setUrl(readValue(INIUNIT_url, index));
                site.setEncode(readValue(INIUNIT_encode, index));
                site.setMethod(readValue(INIUNIT_method, index));
                site.setRegex(readValue(INIUNIT_regex, index));
                site.setClickable(readValue(INIUNIT_clickable, index));

                // sitesに追加
                sites[std::stoi(presetid)] = site;
            }

            return sites;
        }




        void ConfigRepository::StandardRepository::writeCustomSite(const Site& site, int index) const
        {
            if (iniPath().empty())
                return;

            writeValue(INIUNIT_clickable, site.clickable(), index);
            writeValue(INIUNIT_enabled, site.enabled(), index);
            writeValue(INIUNIT_encode, site.encode(), index);
            writeValue(INIUNIT_method, site.method(), index);
            writeValue(INIUNIT_preset, site.preset(), index);
            writeValue(INIUNIT_regex, site.regex(), index);
            writeValue(INIUNIT_shorcut, site.shortcut(), index);
            writeValue(INIUNIT_statusbar, site.statusbar(), index);
            writeValue(INIUNIT_title, site.title(), index);
            writeValue(INIUNIT_url, site.url(), index);
        }

        void ConfigRepository::StandardRepository::writePresetSite(const Site& site, int index) const
        {
            if (iniPath().empty())
                return;

            writeValue(INIUNIT_enabled, site.enabled(), index);
            writeValue(INIUNIT_presetid, site.presetid(), index);
            writeValue(INIUNIT_preset, L"true", index);
        }



        bool ConfigRepository::PresetRepository::initialize(const std::wstring& iniPath)
        {
            if (iniPath.empty())
                return false;

            presetPath_ = iniPath;
            return true;
        }



        const std::wstring& ConfigRepository::PresetRepository::iniPath() const
        {
            return presetPath_;
        }



        bool ConfigRepository::StandardRepository::initialize(const std::wstring & iniPath)
        {
            if (iniPath.empty())
                return false;

            standardPath_ = iniPath;
            return true;
        }



        const std::wstring& ConfigRepository::StandardRepository::iniPath() const
        {
            return standardPath_;
        }

    }
}
