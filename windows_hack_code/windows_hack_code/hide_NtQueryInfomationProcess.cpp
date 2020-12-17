#include "stdafx.h"
#include "hide_NtQueryInfomationProcess.h"


/*
	step 1. open process and get the handler
	step 2. get NtQueryInformationProcess function from ntdll.dll
	step 3. get PEB of target process
	step 4. change PEB information
*/

typedef NTSTATUS (*type_NtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

INT hide_NtQueryInformationProcess(DWORD dwPid, wchar_t * cmd, wchar_t * path) {
	// def
	type_NtQueryInformationProcess NtQueryInformationProcess = NULL;
	PROCESS_BASIC_INFORMATION pbi = { 0 };
	PEB peb = { 0 };
	RTL_USER_PROCESS_PARAMETERS para = { 0 };
	USHORT usCmdLen = 0;
	USHORT usPathLen = 0;
	// step 1
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess == NULL) {
		return GetLastError();
	}
	// step 2
	NtQueryInformationProcess = (type_NtQueryInformationProcess)::GetProcAddress(LoadLibraryA("ntdll.dll"), "NtQueryInformationProcess");
	if (NtQueryInformationProcess == NULL) {
		return GetLastError();
	}
	// step 3
	NTSTATUS ret = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
	if (!NT_SUCCESS(ret)) {
		printf("error: NtQueryInformationProcess\n");
		return -1;
	}
	// get PEB
	ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL);
	// get para
	ReadProcessMemory(hProcess, peb.ProcessParameters, &para, sizeof(para), NULL);
	// step 4
	usCmdLen = 2 + 2 * wcslen(cmd);
	WriteProcessMemory(hProcess, para.CommandLine.Buffer, cmd, usCmdLen, NULL);
	WriteProcessMemory(hProcess, &para.CommandLine.Length, &usCmdLen, sizeof(usCmdLen), NULL);
	usPathLen = 2 + 2 * wcslen(path);
	WriteProcessMemory(hProcess, para.ImagePathName.Buffer, path, usPathLen, NULL);
	WriteProcessMemory(hProcess, &para.ImagePathName.Length, &usPathLen, sizeof(usPathLen), NULL);
	return 1;
}