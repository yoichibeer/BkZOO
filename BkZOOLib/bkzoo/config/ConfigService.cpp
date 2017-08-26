/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "ConfigService.h"

#include "ConfigRepository.h"
#include "util/PathUtils.h"

extern HMODULE hModuleBkZOO;

namespace bkzoo
{
    namespace config
    {

        bool ConfigService::initialize(HMODULE hModule)
        {
            // iniファイルのパスを取得
            const fs::path modulePath = util::PathUtils::modulePath(hModule);
            const fs::path generalPath = util::PathUtils::extensionReplacedPath(modulePath, L"ini");
            const fs::path presetPath = modulePath.parent_path() / modulePath.stem() / fs::path(modulePath).replace_extension(L"preset.ini").filename();

            // iniファイルからデータロード
            if (!ConfigRepository::instance().initialize(generalPath.wstring(), presetPath.wstring()))
                return false;

            // Repository（の中のFactory）でConfig生成
            std::unique_ptr<Config> config(ConfigRepository::instance().loadConfig());

            // configをグローバルにアクセスできるように設定
            Config::setInstance(std::move(config));

            return true;
        }

    }
}
