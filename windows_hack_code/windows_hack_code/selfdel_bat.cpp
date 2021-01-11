#include "stdafx.h"
#include "selfdel_bat.h"

BOOL CreatePingBat(char *pszBatFileName)
{
	int iTime = 5;
	char szBat[MAX_PATH] = { 0 };

	// 构造批处理内容
	/*
	@echo off
	ping 127.0.0.1 -n 5
	del *.exe
	del %0
	*/
	::wsprintf(szBat, "@echo off\nping 127.0.0.1 -n %d\ndel windows_hack_code.exe\ndel %%0\n", iTime);

	// 生成批处理文件
	FILE *fp = NULL;
	fopen_s(&fp, pszBatFileName, "w+");
	if (NULL == fp)
	{
		return FALSE;
	}
	fwrite(szBat, (1 + ::lstrlen(szBat)), 1, fp);
	fclose(fp);

	return TRUE;
}

BOOL DelSelf()
{
	BOOL bRet = FALSE;
	char szCurrentDirectory[MAX_PATH] = { 0 };
	char szBatFileName[MAX_PATH] = { 0 };
	char szCmd[MAX_PATH] = { 0 };

	// 获取当前程序所在目录
	::GetModuleFileName(NULL, szCurrentDirectory, MAX_PATH);
	char *p = strrchr(szCurrentDirectory, '\\');
	p[0] = '\0';
	// 构造批处理文件路径
	::wsprintf(szBatFileName, "%s\\temp.bat", szCurrentDirectory);
	// 构造调用执行批处理的 CMD 命令行
	::wsprintf(szCmd, "cmd /c call \"%s\"", szBatFileName);

	// 创建自删除的批处理文件
	// ping 方式
	bRet = CreatePingBat(szBatFileName);

	// 创建新的进程, 以隐藏控制台的方式执行批处理
	if (bRet)
	{
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);
		//指定wShowWindow成员有效
		si.dwFlags = STARTF_USESHOWWINDOW;
		//此成员设为TRUE的话则显示新建进程的主窗口
		si.wShowWindow = FALSE;
		BOOL bRet = CreateProcess(
			//不在此指定可执行文件的文件名
			NULL,
			//命令行参数
			szCmd,
			//默认进程安全性
			NULL,
			//默认进程安全性
			NULL,
			//指定当前进程内句柄不可以被子进程继承
			FALSE,
			//为新进程创建一个新的控制台窗口
			CREATE_NEW_CONSOLE,
			//使用本进程的环境变量
			NULL,
			//使用本进程的驱动器和目录
			NULL,
			&si,
			&pi);
		if (bRet)
		{
			//不使用的句柄最好关掉
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			// 结束进程
			// exit(0);
			//::ExitProcess(NULL);
		}
	}

	return bRet;
}

