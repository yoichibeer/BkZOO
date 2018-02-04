/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include <windows.h>
#include "MlangApi.h"

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace string
    {

        MlangApi::MlangApi()
            : hModule_(nullptr), isInitialized_(FALSE)
        {
            isInitialized_ = initializeAPI();
        }

        MlangApi::~MlangApi()
        {
            if (hModule_)
            {
                // ::FreeLibrary(hModule_);
                // 上記の ::FreeLibrary(hModule_); がなぜコメントアウトかわからない。
                // どこかのDllMainからこのデストラクタが呼ばれているわけではないはず。
                // だが、Becky!が起動時は最大２回しか呼ばれないし、終了時はシステムがFreeするはずなので
                // とりあえず現状のままコメント。
            }
        }

        BOOL MlangApi::initializeAPI()
        {
            if (hModule_ != nullptr) return TRUE;

            BOOL bRet = FALSE;
            while (true)
            {
                hModule_ = ::LoadLibrary(L"mlang.dll");
                if (!hModule_)
                {
                    return FALSE;
                }
                (FARPROC&)ConvertINetMultiByteToUnicode = GetProcAddress(hModule_,
                    "ConvertINetMultiByteToUnicode");
                if (ConvertINetMultiByteToUnicode == nullptr) break;
                (FARPROC&)ConvertINetUnicodeToMultiByte = GetProcAddress(hModule_,
                    "ConvertINetUnicodeToMultiByte");
                if (ConvertINetUnicodeToMultiByte == nullptr) break;
                bRet = TRUE;
                break;
            }
            if (!bRet)
            {
                hModule_ = nullptr;
            }

            return bRet;
        }


        BOOL MlangApi::isInitialized() const
        {
            return isInitialized_;
        }

    }
}
