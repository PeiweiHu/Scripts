// windows_hack_code.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "single_instance.h"
#include "resource.h"
#include "free_resource.h"

void test_free_resource();
void test_single_instance();

int _tmain(int argc, _TCHAR* argv[]) {
	
}


void test_free_resource() {
	INT ret = free_res(IDR_RES1, "RES", "C:\\Users\\***\\Desktop\\releaseme.txt");
	if (ret > 0) {
		printf("success\n");
	}
	else {
		printf("%d\n", ret);
	}
}

void test_single_instance() {
	if (firstRun((LPCTSTR)"test") == 1) {
		printf("first time\n");
		while (1);
	}
	else {
		printf("already exists");
	}
	return;
}
