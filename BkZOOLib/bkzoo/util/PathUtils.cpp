/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "PathUtils.h"

#include <regex>

namespace bkzoo
{
    namespace util
    {

        fs::path PathUtils::modulePath(HMODULE hModule)
        {
            wchar_t filename[_MAX_PATH] = { 0 };
            const DWORD length = ::GetModuleFileName(hModule, filename, _MAX_PATH);
            if (length == 0)
                return L"";

            return filename;
        }

        fs::path PathUtils::extensionReplacedPath(const::fs::path& sourcePath, const std::wstring& newext)
        {
            if (sourcePath.empty())
                return L""; // replace_extension()だとL""にならないのでこの関数を実装。

            fs::path replacedPath(sourcePath);
            return replacedPath.replace_extension(newext);
        }

    }
}
