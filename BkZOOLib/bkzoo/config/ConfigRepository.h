/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_CONFIG_CONFIGREPOSITORY_H
#define BKZ_CONFIG_CONFIGREPOSITORY_H

#include "Config.h"

#include <Windows.h>

#include <string>
#include <map>

#include "bkzoo_gtest.h"
#include "defs.h"


FORWARD_DECLARE_CLASS_GTEST(ConfigRepository, defaultValue);
FORWARD_DECLARE_CLASS_GTEST(ConfigRepository, readGeneral);
FORWARD_DECLARE_CLASS_GTEST(ConfigRepository, readPresetSites);
FORWARD_DECLARE_CLASS_GTEST(ConfigRepository, readStandardSites);
FORWARD_DECLARE_CLASS_GTEST(ConfigRepository, writeGeneral);
FORWARD_DECLARE_CLASS_GTEST(ConfigRepository, writeStandardSites);
FORWARD_DECLARE_CLASS_GTEST(ConfigRepository, removeStandardSites);

namespace bkzoo
{
    namespace config
    {

        /// ::GetPrivateProfileString()や::WritePrivateProfileString()で使うパラメータのセット
        typedef struct final
        {
            std::wstring appName;
            std::wstring keyName;
            std::wstring defaultValue;
        } IniUnit;

        class ConfigRepository final
        {
            FRIEND_GTEST(ConfigRepository, defaultValue);
            FRIEND_GTEST(ConfigRepository, readGeneral);
            FRIEND_GTEST(ConfigRepository, readPresetSites);
            FRIEND_GTEST(ConfigRepository, readStandardSites);
            FRIEND_GTEST(ConfigRepository, writeGeneral);
            FRIEND_GTEST(ConfigRepository, writeStandardSites);
            FRIEND_GTEST(ConfigRepository, removeStandardSites);

        public:
            static ConfigRepository& instance();

            /// @todo (yoichi) i18nはここで指定する。
            /// @param[in] standardPath BkZOOが標準で使う設定ファイルのパス
            /// @param[in] presetPath プリセットサイトを格納した読み取り専用の設定ファイルのパス
            bool initialize(const std::wstring& standardPath, const std::wstring& presetPath);

            std::unique_ptr<Config> loadConfig() const;
            bool saveConfig(const Config& config) const;

        private:

            /// @todo (yoichi) 継承やめたほうが良い。staticメソッドか委譲するか。
            class IniRepository
            {
            public:
                IniRepository() = default;
                virtual ~IniRepository() = default;

                std::wstring readValue(const IniUnit& iniUnit) const;

                /// @param[in] index iniファイルのセレクターのindex。具体的には[Site3]の3の部分。
                std::wstring readValue(const IniUnit& iniUnit, int index) const;

                BOOL writeValue(const IniUnit& iniUnit, const std::wstring& value) const;
                BOOL writeValue(const IniUnit& iniUnit, const std::wstring& value, int index) const;

                SiteVector readStandardSites() const;
                SiteMap readPresetSites() const;

            private:
                virtual const std::wstring& iniPath() const = 0;

            private:
                DISALLOW_COPY_AND_ASSIGN(IniRepository);
            };

            class StandardRepository final : public IniRepository
            {
            public:
                StandardRepository() = default;
                virtual ~StandardRepository() = default;

                bool initialize(const std::wstring& iniPath);
                virtual const std::wstring& iniPath() const override;

                void writeCustomSite(const Site& site, int index) const;
                void writePresetSite(const Site& site, int index) const;

            private:
                std::wstring standardPath_;
            };

            class PresetRepository final : public IniRepository
            {
            public:
                PresetRepository() = default;
                virtual ~PresetRepository() = default;

                bool initialize(const std::wstring& iniPath);
                virtual const std::wstring& iniPath() const override;

            private:
                std::wstring presetPath_;
            };

        private:
            General readGeneral() const;
            SiteMap readPresetSites() const;
            SiteVector readStandardSites() const;

            void writeStandardSites(const SiteList& customSites) const;
            void writeGeneral(const General& general) const;

            void setShortcutToGeneral(General* general, App app) const;
            void writeShortcutToStandardIni(const General& general, App app) const;
            void removeStandardSites() const;

            StandardRepository standardRepository_;
            PresetRepository presetRepository_;

            ConfigRepository() = default;
            ~ConfigRepository() = default;

            DISALLOW_COPY_AND_ASSIGN(ConfigRepository);
        };

    }
}

#endif // BKZ_CONFIG_CONFIGREPOSITORY_H
