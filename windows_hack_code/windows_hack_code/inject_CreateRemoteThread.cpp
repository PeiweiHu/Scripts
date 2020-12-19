#include "stdafx.h"

/*
	1. get target process handler
	2. apply for space from target process and write dll path to it
	3. get address of function LoadLibrary in target process, it bases on kernel32.dll ( it contains LoadLibrary ) has the same address in different process
	4. create a new thread in target process 
*/

INT inject_CreateRemoteThread(DWORD process_id, char * dll_path) {
	HANDLE hProcess = NULL;
	DWORD dwSize = 0;
	LPVOID pAddr = NULL;
	FARPROC pLoadLibrary = NULL;
	// step 1
	hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
	if (hProcess == NULL) {
		return ::GetLastError();
	}
	// step 2
	dwSize = ::lstrlenA(dll_path) + 1;
	pAddr = ::VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (pAddr == NULL) {
		return -2;
	}
	if (::WriteProcessMemory(hProcess, pAddr, dll_path, dwSize, NULL) == FALSE) {
		return -3;
	}
	// step 3
	pLoadLibrary = ::GetProcAddress(::GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
	if (pLoadLibrary == NULL) {
		return -4;
	}
	// step 4
	HANDLE hThread = ::CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pAddr, 0, NULL);
	if (hThread == NULL) {
		return ::GetLastError();
	}
	::CloseHandle(hProcess);
	return 1;
}