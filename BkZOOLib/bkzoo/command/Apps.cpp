/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "Apps.h"

#include "registry/Registry.h"
#include "bkzoo_url.h"
#include "util/MessageBox.h"

#include "resource.h"

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        // FFFTP用のParam生成
        AppsParam AppsParam::createParamOfFFFTP(HWND hWnd, const std::wstring& selectedString)
        {
            AppsParam param{ hWnd, selectedString, L"FFFTP", L"\\FFFTP.exe",
                Scheme::FTP, Scheme::FTP };
            return param;
        }

        // FileZilla用のParam生成
        AppsParam AppsParam::createParamOfFileZilla(HWND hWnd, const std::wstring& selectedString)
        {
            AppsParam param{ hWnd, selectedString, L"FileZilla Client", L"\\filezilla.exe",
                Scheme::FTP | Scheme::FTPS | Scheme::SFTP, Scheme::FTP };
            return param;
        }

        // WinSCP用のParam生成
        AppsParam AppsParam::createParamOfWinSCP(HWND hWnd, const std::wstring& selectedString)
        {
            AppsParam param{ hWnd, selectedString, L"winscp3_is1", L"\\WinSCP.exe",
                Scheme::FTP | Scheme::SFTP | Scheme::SCP, Scheme::SCP };
            return param;
        }

        // TeraTerm用のParam生成
        AppsParam AppsParam::createParamOfTeraTerm(HWND hWnd, const std::wstring& selectedString)
        {
            AppsParam param{ hWnd, selectedString, L"Tera Term_is1", L"\\ttermpro.exe",
                Scheme::TELNET, Scheme::TELNET };
            return param;
        }



        AppsCommand::AppsCommand(const AppsParam& param)
            : param_(param)
        {
        }

        bool AppsCommand::execute()
        {
            // 指定するアプリが存在するか確認
            std::wstring installLocation;
            bool exists = registry::Registry::getInstallLocation(param_.installLocationKey.c_str(), &installLocation);
            if (!exists)
            {
                return false;
            }

            // url取得
            const url::URL url(param_.selectedString, param_.availableSchemes);
            if (!url.validate())
            {
                return false;
            }
            const std::wstring url_str = url.getURL();

            //// 警告表示
            //if (! util::okCancelWarningMessageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_WARNING_OPEN_URL))
            //{
            //    // skip
            //    return true;
            //}

            // 指定したアプリでurlを開く
            installLocation.append(param_.installLocationFileName);
            ::ShellExecute(param_.hWnd, nullptr, installLocation.c_str(),
                url_str.c_str(), nullptr, SW_SHOW);

            return true;
        }

    }
}
