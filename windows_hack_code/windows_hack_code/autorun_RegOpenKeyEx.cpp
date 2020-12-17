#include "stdafx.h"
#pragma comment(lib, "advapi32") // link this lib for regedit related api

INT autorun_RegOpenKeyEx(char * autorun_exe, char * reg_name) {
	HKEY hKey;
	if (::RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
		return ::GetLastError();
	}
	if (::RegSetValueExA(hKey, reg_name, 0, REG_SZ, (BYTE *)autorun_exe, 1 + ::lstrlenA(autorun_exe)) != ERROR_SUCCESS) {
		return ::GetLastError();
	}
	::RegCloseKey(hKey);
	return 0;
}