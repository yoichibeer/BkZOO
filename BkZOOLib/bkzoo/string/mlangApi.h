/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_STRINGS_MLANG_API_H
#define BKZ_STRINGS_MLANG_API_H

#include <Windows.h>

#include "defs.h"

namespace bkzoo
{
    namespace string
    {

        class MlangApi final
        {
        public:
            MlangApi();

            ~MlangApi();

            BOOL isInitialized() const;

            HRESULT(WINAPI* ConvertINetMultiByteToUnicode)(LPDWORD lpdwMode, DWORD dwEncoding,
                LPCSTR lpSrcStr, LPINT lpnMultiCharCount, LPWSTR lpDstStr, LPINT lpnWideCharCount);
            HRESULT(WINAPI* ConvertINetUnicodeToMultiByte)(LPDWORD lpdwMode, DWORD dwEncoding,
                LPCWSTR lpSrcStr, LPINT lpnWideCharCount, LPSTR lpDstStr, LPINT lpnMultiCharCount);

        private:
            HMODULE hModule_;
            BOOL isInitialized_;

        private:
            BOOL initializeAPI();

            DISALLOW_COPY_AND_ASSIGN(MlangApi);
        };

    }
}

#endif // BKZ_STRINGS_MLANG_API_H
