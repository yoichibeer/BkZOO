/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef WTL_HEADER_H
#define WTL_HEADER_H

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
 // Windows ヘッダー ファイル:
#include <windows.h>


#include <atlbase.h>

#pragma warning(push)
#pragma warning(disable:4996) // for GetVersionExW
#include <atlapp.h>
#pragma warning(pop)

#include <atlcrack.h>
#include <atlctrls.h>
#include <atlframe.h>
#include <atlmisc.h>
#include <atlwin.h>


#endif // WTL_HEADER_H
