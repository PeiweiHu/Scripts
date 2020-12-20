#include "stdafx.h"
#include "enum_process_CreateToolhelp32Snapshot.h"

INT EnumProcess() {
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// create snapshot
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		return GetLastError();
	}
	// start enum
	BOOL bRet = ::Process32First(hProcessSnap, &pe32);
	while (bRet) {
		printf("[%d]\t", pe32.th32ProcessID);
		printf("%s\n", pe32.szExeFile);
		bRet = ::Process32Next(hProcessSnap, &pe32);
	}
	::CloseHandle(hProcessSnap);
	return 0;
}