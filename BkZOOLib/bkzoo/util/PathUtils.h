/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_PATH_H
#define BKZ_UTIL_PATH_H

#include <Windows.h>

#include <string>

#include "defs.h"

#include <filesystem>
namespace fs = std::experimental::filesystem;

namespace bkzoo
{
    namespace util
    {

        class PathUtils final
        {
        public:
            static fs::path modulePath(HMODULE hModule);
            static fs::path extensionReplacedPath(const fs::path& sourcePath, const std::wstring& newext);
        private:
            PathUtils() = delete;
            ~PathUtils() = delete;
            DISALLOW_COPY_AND_ASSIGN(PathUtils);
        };

    }
}

#endif // BKZ_UTIL_PATH_H
