/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "util/PathUtils.h"
#include "mouse/MouseHook.h"

#include "config/ConfigService.h"
#include "config/Config.h"

#include "bkzoo_log.h"

#include <BeckyAPI.h>

#include "for_debug/detect_memory_leak.h"

HMODULE hModuleBkZOO = nullptr;

HHOOK hHookMouse = nullptr;

CBeckyAPI beckyApi;

/////////////////////////////////////////////////////////////////////////////
// DLL entry point
// cppcheck-suppress unusedFunction
BOOL APIENTRY DllMain(HANDLE hModule,
    DWORD ul_reason_for_call,
    LPVOID  /*lpReserved*/
)
{
#ifdef _DEBUG
    ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //	_CrtSetBreakAlloc( 5973 );
#endif //_DEBUG

    ::hModuleBkZOO = static_cast<HMODULE>(hModule);
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        try
        {
            // BeckyAPI 初期化
            if (!::beckyApi.InitAPI())
            {
                return FALSE;
            }


            // Config設定
            bkzoo::config::ConfigService::initialize(::hModuleBkZOO);


            // Mouse Hook 設定
            ::hHookMouse = ::SetWindowsHookEx(WH_MOUSE, bkzoo::mouse::MouseProc, ::hModuleBkZOO, ::GetCurrentThreadId());


            // ログ設定
            const fs::path modulePath = ::bkzoo::util::PathUtils::modulePath(::hModuleBkZOO);
            const fs::path logPath = modulePath.parent_path() / modulePath.stem() / modulePath.filename().replace_extension(L".log");
            Logger::createInstanceCallOnceFirst(logPath.wstring());
            Logger::instance().setMaximumOutputLogLevel(bkzoo::config::Config::instance().logLevel());
        }
        catch (...)
        {
            assert(false);
        }
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
