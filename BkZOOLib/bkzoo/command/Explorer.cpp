/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Explorer.h"

#include "util/MessageBox.h"
#include "bkzoo_url.h"
#include "bkzoo_util.h"

#include "resource.h"

#include "bkzoo_log.h"
#include "bkzoo_string.h"

#include <filesystem>

#include <regex>
#include <sstream>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        ExplorerCommand::ExplorerCommand(const SimpleParam& param)
            : param_(param)
        {
            LOG_INFO << "param.hWnd=" << param.hWnd << "param.string=" << wm_cast<std::string>(param.string);
        }

        bool ExplorerCommand::execute()
        {
            LOG_INFO << "----START----";
            const url::URL url(param_.string, Scheme::FILE_URI_LOCALPATH | Scheme::FILE_URI_UNC);
            if (!url.validate())
            {
                LOG_WARNING << "param_.hWnd=" << param_.hWnd << "param_.string=" << wm_cast<std::string>(param_.string);
                return false;
            }

            // url取得
            bool truncatedFileUri = false;
            std::wstring urlString = url.getURLWith(&truncatedFileUri);

            // ディレクトリかファイルかを判定してShellExecute呼び出し時のoperationを決定
            const std::wregex fileOrDirRegex(L"^[fF][iI][lL][eE]:?([\\\\/]{2,3})([^\\\\/].*)$");
            std::wsmatch resultsFileOrDir;
            if (!std::regex_search(urlString, resultsFileOrDir, fileOrDirRegex))
            {
                LOG_ERROR << "urlString=" << wm_cast<std::string>(urlString);
                assert(false);
                return false;
            }
            std::wostringstream fileOrDir;
            if (3 == resultsFileOrDir[1].str().length())
            {
                // file:///C:\abc の場合
                fileOrDir << resultsFileOrDir[2].str();
            }
            else if (2 == resultsFileOrDir[1].str().length())
            {
                fileOrDir << L"\\\\" << resultsFileOrDir[2].str();
            }
            else
            {
                LOG_ERROR << "fileOrDir=" << fileOrDir.str();
                assert(false);
                return false;
            }
            const std::wstring fileOrDirString = fileOrDir.str();
            const std::wregex onlyComputerNameRegex(L"^[\\\\/]{2}[^\\\\/]+$");
            if (!std::regex_match(fileOrDirString, onlyComputerNameRegex))
            {
                // \\の後がcomputer nameだけの場合はexistsでfalseになってしまうためそれ以外だけチェック
                const std::experimental::filesystem::path fileOrDirPath(fileOrDirString);
                if (!std::experimental::filesystem::exists(fileOrDirPath))
                {
                    LOG_ERROR << "fileOrDirPath=" << fileOrDirPath.string();
                    assert(false);
                    return false;
                }
            }

            // 警告表示
            //if (!util::okCancelWarningMessageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_WARNING_OPEN_URL))
            //{
            //    // skip
            //    return true;
            //}

            if (truncatedFileUri)
            {
                util::messageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_TRUNCATED_FILE_URI);
            }

            // urlStringからfile:を除去したのがfileOrDirString。
            // file: があると # を含むパスをエクスプローラーで開けないため file: を除去したパスを ShellExecute に渡す。
            const HINSTANCE hInstance = ::ShellExecute(param_.hWnd, L"open", fileOrDirString.c_str(), nullptr, nullptr, SW_SHOW);
            if (reinterpret_cast<const size_t>(hInstance) <= 32U)
            {
                LOG_ERROR << "ShellExecute() : " << reinterpret_cast<const size_t>(hInstance);
            }

            LOG_INFO << " -----END-----";
            return true;
        }

    }
}
