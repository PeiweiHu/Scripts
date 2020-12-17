#include "stdafx.h"
#include "autorun_SHGetSpecialFolderPath.h"

#pragma comment (lib, "shell32.lib")
#pragma comment (lib, "user32.lib")


INT autorun_SHGetSpecialFolderPath(char * src_path, char * dst_file_name) {
	char startup_path[MAX_PATH] = { 0 };
	// dst_path = startup_path + dst_file_name
	char dst_path[MAX_PATH] = { 0 };
	// step 1. get start up dir
	if (SHGetSpecialFolderPathA(NULL, startup_path, CSIDL_STARTUP, TRUE) == FALSE) {
		return GetLastError();
	}
	printf("start up path: %s\n", startup_path);
	// step 2. construct dst path
	::wsprintfA(dst_path, "%s\\%s", startup_path, dst_file_name);
	// step 3. copy to start up dir
	if (CopyFileA(src_path, dst_path, FALSE) == FALSE) {
		return GetLastError();
	}
	return 0;
}