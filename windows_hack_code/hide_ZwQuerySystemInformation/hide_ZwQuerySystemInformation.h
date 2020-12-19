#include "stdafx.h"


NTSTATUS NewZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLen,
	PULONG RtnLen
	);

void HookApi();
void UnHookApi();