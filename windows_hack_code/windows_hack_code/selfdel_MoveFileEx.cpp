#include "stdafx.h"
#include "selfdel_MoveFileEx.h"

BOOL selfdel_move(char * pszFileName) {
	char szTemp[MAX_PATH] = "\\\\?\\";
	::lstrcat(szTemp, pszFileName);
	BOOL bRet = ::MoveFileEx(szTemp, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	return bRet;
}