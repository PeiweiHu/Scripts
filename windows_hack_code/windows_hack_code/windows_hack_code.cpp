// windows_hack_code.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "single_instance.h"
#include "resource.h"
#include "free_resource.h"
#include "inject_CreateRemoteThread.h"
#include "run_WinExec.h"
#include "autorun_RegOpenKeyEx.h"
#include "autorun_SHGetSpecialFolderPath.h"

void test_free_resource();
void test_single_instance();
void test_SetWindowsHookEx();
void test_CreateRemoteThread();
void test_WinExec();
void test_RegOpenKeyEx();
void test_SHGetSpecialFolderPath();

int _tmain(int argc, _TCHAR* argv[]) {

}

void test_SHGetSpecialFolderPath() {
	autorun_SHGetSpecialFolderPath("C:\\Users\\****\\Desktop\\run.bat", "run.bat");
}

void test_RegOpenKeyEx() {
	INT ret = autorun_RegOpenKeyEx("C:\\Users\\****\\Desktop\\run.bat", "REG_TEST");
	printf("ret: %d", ret);
}

void test_WinExec() {
	BOOL ret = run_WinExec("path/to/exe arg", SW_SHOWNORMAL);
	printf("%d\n", ret);
}

void test_CreateRemoteThread() {
	DWORD pid = 3960;
	INT res = inject_CreateRemoteThread(pid, "\\path\\to\\dll");
	printf("%d\n", res);
	return;
}

void test_SetWindowsHookEx() {
	typedef bool(*BOOL_FUNC)();
	HMODULE dll = ::LoadLibraryA("inject_SetWindowsHookEx");
	if (dll == NULL) {
		printf("Fail to load dll, error code: %d\n", ::GetLastError());
		return;
	}
	BOOL_FUNC GlobalHook = (BOOL_FUNC)::GetProcAddress(dll, "GlobalHook");
	BOOL_FUNC UnGlobalHook = (BOOL_FUNC)::GetProcAddress(dll, "UnGlobalHook");
	BOOL ret = GlobalHook();
	if (ret == FALSE) {
		printf("Fail to hook\n");
		return;
	} else {
		printf("Success to hook\n");
	}
	system("pause");
	ret = UnGlobalHook();
	if (ret == TRUE) {
		printf("Success to unhook\n");
	} else {
		printf("Fail to unhook\n");
		return;
	}
	return;
}

// don't forget to attach resource first while testing
void test_free_resource() {
	INT ret = free_res(IDR_RES1, "RES", "C:\\Users\\***\\Desktop\\releaseme.txt");
	if (ret > 0) {
		printf("success\n");
	}
	else {
		printf("%d\n", ret);
	}
}

void test_single_instance() {
	if (firstRun((LPCTSTR)"test") == 1) {
		printf("first time\n");
		while (1);
	}
	else {
		printf("already exists");
	}
	return;
}
