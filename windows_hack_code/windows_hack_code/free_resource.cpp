#include "stdafx.h"
#include "free_resource.h"

INT free_res(UINT res_name, char * res_type, char * output) {
	// find handler of target resource
	HRSRC hRsrc = ::FindResourceA(NULL, MAKEINTRESOURCEA(res_name), res_type);
	if (hRsrc == NULL) {
		printf("%d", GetLastError());
		return -1;
	}
	// load resource
	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (hGlobal == NULL) return -2;
	// lock resource
	LPVOID lpVoid = ::LockResource(hGlobal);
	if (lpVoid == NULL) return -3;
	// get size of resource
	DWORD dwSize = ::SizeofResource(NULL, hRsrc);
	if (dwSize <= 0) return -4;
	// write resource to output
	FILE * fp = NULL;
	fopen_s(&fp, output, "wb+");
	if (fp == NULL) return -5;
	fwrite(lpVoid, sizeof(char), dwSize, fp);
	fclose(fp);
	return 1;
}