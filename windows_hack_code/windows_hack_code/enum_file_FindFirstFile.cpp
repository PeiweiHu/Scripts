#include "stdafx.h"

void SearchFile(char * directory) {
	DWORD dwBufSize = 2048;
	char * name = new char[dwBufSize];
	char * tmp = new char[dwBufSize];
	WIN32_FIND_DATA data = { 0 };
	BOOL bRet = FALSE;
	::wsprintf(name, "%s\\*.*", directory);
	HANDLE hFile = ::FindFirstFile(name, &data);
	if (hFile != INVALID_HANDLE_VALUE) {
		do {
			// contains . and ..
			if (data.cFileName[0] == '.') {
				continue;
			}
			// construct file path
			::wsprintf(tmp, "%s\\%s", directory, data.cFileName);
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				SearchFile(tmp);
			} else {
				printf("%s\n", tmp);
			}
		} while (::FindNextFile(hFile, &data));
	}
	::FindClose(hFile);
	delete [] tmp;
	delete [] name;
	tmp = NULL;
	name = NULL;
}