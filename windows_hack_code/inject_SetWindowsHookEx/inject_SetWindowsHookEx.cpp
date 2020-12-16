// inject_SetWindowsHookEx.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

extern HMODULE g_hDllModule;
// shared memory, so different loader dll can share hook handler
#pragma data_seg("hook_handler")
HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:hook_handler,RWS")

LRESULT CallBack(int code, WPARAM wParam, LPARAM lParam) {
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}

BOOL GlobalHook() {
	g_hHook = ::SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)CallBack, g_hDllModule, 0);
	if (g_hHook = NULL) return FALSE;
	return TRUE;
}

BOOL UnGlobalHook() {
	if (g_hHook) {
		::UnhookWindowsHookEx(g_hHook);
	}
	return TRUE;
}



