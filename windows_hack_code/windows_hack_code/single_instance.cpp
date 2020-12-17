#include "stdafx.h"
#include "single_instance.h"

INT firstRun(char * name) {
	HANDLE hMutex = NULL;
	hMutex = ::CreateMutexA(NULL, false, name);
	if (hMutex) {
		if (::GetLastError() == ERROR_ALREADY_EXISTS) {
			return 0;
		}
		return 1;
	}
	return -1;
}