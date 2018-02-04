/*
 * BkZOO!
 *
 * Copyright 2011-2018 yoichibeer.
 * Released under the MIT license.
 */

#include "InternetExplore.h"

#include "registry\Registry.h"
#include "util/MessageBox.h"
#include "bkzoo_url.h"
#include "bkzoo_log.h"
#include "bkzoo_string.h"

#include "resource.h"

#include <Exdisp.h>
#include <comutil.h>
 //#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "comsuppw.lib")
#include <comdef.h>
#include <comdefsp.h>
#include <atlbase.h> // for CComPtr

#include "debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace command
    {

        // IE起動時はShellExecuteを使わずにNavigate2()で新規Tab起動。
        // IE起動していなければShellExecuteで新規Window起動。
        // IE起動しているときにCoCreateInstance()とNavigate2()を使って新規Windowにすると、
        // 余計なTabを開く（MSN）のでやめてShellExecuteにした。
        static void OpenIexplore(const std::string& path, HWND hWnd)
        {
            CComPtr<IShellWindows> iShellWindows;
            HRESULT hr = ::CoCreateInstance(CLSID_ShellWindows, nullptr, CLSCTX_ALL, IID_IShellWindows, (void**)&iShellWindows);
            if (!SUCCEEDED(hr))
                return;

            long count = 0;
            hr = iShellWindows->get_Count(&count);
            if (!SUCCEEDED(hr))
                return;

            CComPtr<IWebBrowser2> foundIWebBrowser2;
            bool found = false;
            for (long i = 0; i < count; ++i) {
                const _variant_t index(i, VT_I4);
                CComPtr<IDispatch> window;
                hr = iShellWindows->Item(index, &window);
                if (hr != S_OK)
                    continue;

                CComPtr<IWebBrowser2> iWebBrowser2;
                hr = window->QueryInterface(IID_IWebBrowserApp, (void **)&iWebBrowser2);
                if (!SUCCEEDED(hr))
                    return;

                BSTR fullName;
                iWebBrowser2->get_FullName(&fullName);
                std::wstring fullNameStr(fullName);
                if (fullNameStr.find(L"IEXPLORE") == std::string::npos)
                {
                    continue;
                }
                else
                {
                    found = true;
                    foundIWebBrowser2 = iWebBrowser2;
                    break;
                }
            }

            if (!found)
            {
                // 既に開いているIEのウィンドウがなかったのでIEを起動する。
                std::wstring iexploreInstallLocation;
                if (registry::Registry::getIexploreInstallLocation(&iexploreInstallLocation))
                    ::ShellExecute(hWnd, L"open", iexploreInstallLocation.c_str(), wm_cast<std::wstring>(path).c_str(), nullptr, SW_SHOW);
                else
                    LOG_ERROR << "Not found IEXPLORE!";

                // 上記の ShellExecute でなくて CoCreateInstance で新規IEを開くコードはこちらだが、
                // 余計なTabを開くのでやめた。
                //hr = ::CoCreateInstance(CLSID_InternetExplorer, nullptr, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&foundWebBrowser2);
                //if (!SUCCEEDED(hr))
                //    return;

                return;
            }

            _variant_t url;
            url.SetString(path.c_str());

            _variant_t flags;
            flags.ChangeType(VT_I4, &flags);
            flags.intVal = 0x800; // 既に開いているIEがあれば、新しいTabに開きたいので 0x800 を指定する。ちなみに新しいWindowを開くときは 0x01 を指定する。
            assert(found);

            _variant_t empty;
            VariantInit(&empty);
            hr = foundIWebBrowser2->Navigate2(&url, &flags, &empty, &empty, &empty);
            if (SUCCEEDED(hr))
            {
                foundIWebBrowser2->put_Visible(VARIANT_TRUE);

                // IEを前面に表示
                SHANDLE_PTR hWndWebBrowser = 0;
                foundIWebBrowser2->get_HWND(&hWndWebBrowser);
                ::SwitchToThisWindow(reinterpret_cast<HWND>(hWndWebBrowser), TRUE);
            }
        }



        InternetExploreCommand::InternetExploreCommand(const SimpleParam& param)
            : param_(param), m_availableSchemes(Scheme::HTTP | Scheme::HTTPS | Scheme::FTP)
        {
        }
        InternetExploreCommand::InternetExploreCommand(const SimpleParam& param, unsigned long availableSchemes)
            : param_(param), m_availableSchemes(availableSchemes)
        {
        }

        bool InternetExploreCommand::execute()
        {
            const url::URL url(param_.string, m_availableSchemes);

            if (!url.validate())
            {
                return false;
            }

            // 警告表示
            if (!util::okCancelWarningMessageBox(param_.hWnd, IDS_STRING_ERR_MESSAGE_WARNING_OPEN_URL))
            {
                // skip
                return true;
            }

            // COM使用開始
            ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

            // 単純にShellExecuteだと既にIE起動時に新規Tabでなくて新規Windowで開いてしまうので関数つくった。
            OpenIexplore(wm_cast<std::string>(url.getURL()), param_.hWnd);

            // COM使用終了
            ::CoUninitialize();

            return true;
        }

    }
}
