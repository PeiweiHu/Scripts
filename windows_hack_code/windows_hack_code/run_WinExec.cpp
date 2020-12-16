#include "stdafx.h"

BOOL run_WinExec(char * cmd, UINT show) {
	UINT ret = ::WinExec(cmd, show);
	if (ret > 31) {
		return TRUE;
	} else {
		printf("Error code: %d\n", ::GetLastError());
		return FALSE;
	}
 }