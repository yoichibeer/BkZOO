/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "MouseHook.h"

#include "command/DoubleClick.h"
#include "command/TripleClick.h"
#include "thread_func/ThreadFunc.h"
#include "util/TripleClickDetector.h"
#include "util/ModifierKeyType.h"

#include "bkzoo_config.h"
#include "bkzoo_log.h"

#include <process.h>

#include "debug/detect_memory_leak.h"

extern HHOOK hHookMouse;

// triple click
::bkzoo::util::TripleClickDetector tripleClickDetector;

namespace bkzoo
{
    namespace mouse
    {

        enum class ClickType
        {
            UN_DETECTED,
            SingleClick,
            DoubleClick,
            TripleClick,
        };

        LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
        {
            // MouseProc は同じスレッドで呼ばれるのが下記で確認できる。コメントアウト。
            // DWORD threadID = ::GetCurrentThreadId();

            if (code < 0)
            {
                return ::CallNextHookEx(::hHookMouse, code, wParam, lParam);
            }

            // ModifierKeyType確定
            unsigned long modifierKey = ModifierKeyType::UNDEFINED;
            if (::GetKeyState(VK_SHIFT) & 0x8000)
            {
                modifierKey |= ModifierKeyType::Shift;
            }
            if (::GetKeyState(VK_CONTROL) & 0x8000)
            {
                modifierKey |= ModifierKeyType::Ctrl;
            }
            // Altキーは使わないのでコメントアウトしておく
            //if (::GetKeyState(VK_MENU) & 0x8000){
            //    modifierKey |= ModifierKeyType_Shift;
            //}

            // clickType確定
            ClickType clickType = ClickType::UN_DETECTED;
            MOUSEHOOKSTRUCT* pMhs = (MOUSEHOOKSTRUCT*)lParam;
            if (wParam == WM_LBUTTONDOWN)
            {
                // WM_LBUTTONDOWN はここ（メインスレッド）でTripleClick判定する
                LOG_INFO << "MouseProc() WM_LBUTTONDOWN";
                if (::tripleClickDetector.interrupt(pMhs->pt))
                {
                    LOG_INFO << "MouseProc() ClickType::TripleClick";
                    clickType = ClickType::TripleClick;
                }
                else
                {
                    LOG_INFO << "MouseProc() ClickType::SingleClick";
                    clickType = ClickType::SingleClick;
                }
            }
            else if (wParam == WM_LBUTTONDBLCLK)
            {
                // WM_LBUTTONDBLCLK は更にDoubleClickTime経過後にTripleClickではないことを判定する必要があり、
                // 別スレッドで判定＆ダブルクリックで実行する処理を行う
                LOG_INFO << "MouseProc() WM_LBUTTONDBLCLK";
                ::tripleClickDetector.start(pMhs->pt);
                command::Command* command = new command::DoubleClickCommand(command::DoubleClickParam{ pMhs->hwnd, modifierKey });
                _beginthread(thread_func::executeCommand, 0, command);
                return ::CallNextHookEx(::hHookMouse, code, wParam, lParam);
            }
            else
            {
                // 他のメッセージの場合何もしない
                return ::CallNextHookEx(::hHookMouse, code, wParam, lParam);
            }


            // Shiftキー処理
            if (modifierKey & ModifierKeyType::Shift)
            {
                if (clickType == ClickType::TripleClick)
                {
                    // トリプルクリックで一行選択したいだけで、他になにか開いたり実行したくないときにShiftキーをおしながらトリプルクリックする。
                    return ::CallNextHookEx(::hHookMouse, code, wParam, lParam);
                }
            }


            // TripleClick時の処理
            if (clickType == ClickType::TripleClick)
            {
                if (Config::instance().tripleClick() || Config::instance().ctrlTripleClick())
                {
                    command::Command *command =
                        new command::TripleClickCommand(command::TripleClickParam{ command::SimpleParam{ pMhs->hwnd, /* selectedString = */ L"" }, modifierKey }); // selectedStringはここで取得しても空なので別スレッドで取得する
                    _beginthread(thread_func::executeCommand, 0, command);
                    return ::CallNextHookEx(::hHookMouse, code, wParam, lParam);
                }
            }

            return ::CallNextHookEx(::hHookMouse, code, wParam, lParam);
        }

    }
}
