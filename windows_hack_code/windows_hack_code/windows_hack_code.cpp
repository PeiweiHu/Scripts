// windows_hack_code.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "single_instance.h"


int _tmain(int argc, _TCHAR* argv[]) {
	
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
