#include "stdafx.h"
#include "selfdel_bat.h"

BOOL CreatePingBat(char *pszBatFileName)
{
	int iTime = 5;
	char szBat[MAX_PATH] = { 0 };

	// ��������������
	/*
	@echo off
	ping 127.0.0.1 -n 5
	del *.exe
	del %0
	*/
	::wsprintf(szBat, "@echo off\nping 127.0.0.1 -n %d\ndel windows_hack_code.exe\ndel %%0\n", iTime);

	// �����������ļ�
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

	// ��ȡ��ǰ��������Ŀ¼
	::GetModuleFileName(NULL, szCurrentDirectory, MAX_PATH);
	char *p = strrchr(szCurrentDirectory, '\\');
	p[0] = '\0';
	// �����������ļ�·��
	::wsprintf(szBatFileName, "%s\\temp.bat", szCurrentDirectory);
	// �������ִ��������� CMD ������
	::wsprintf(szCmd, "cmd /c call \"%s\"", szBatFileName);

	// ������ɾ�����������ļ�
	// ping ��ʽ
	bRet = CreatePingBat(szBatFileName);

	// �����µĽ���, �����ؿ���̨�ķ�ʽִ��������
	if (bRet)
	{
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi;
		si.cb = sizeof(si);
		//ָ��wShowWindow��Ա��Ч
		si.dwFlags = STARTF_USESHOWWINDOW;
		//�˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵�������
		si.wShowWindow = FALSE;
		BOOL bRet = CreateProcess(
			//���ڴ�ָ����ִ���ļ����ļ���
			NULL,
			//�����в���
			szCmd,
			//Ĭ�Ͻ��̰�ȫ��
			NULL,
			//Ĭ�Ͻ��̰�ȫ��
			NULL,
			//ָ����ǰ�����ھ�������Ա��ӽ��̼̳�
			FALSE,
			//Ϊ�½��̴���һ���µĿ���̨����
			CREATE_NEW_CONSOLE,
			//ʹ�ñ����̵Ļ�������
			NULL,
			//ʹ�ñ����̵���������Ŀ¼
			NULL,
			&si,
			&pi);
		if (bRet)
		{
			//��ʹ�õľ����ùص�
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			// ��������
			// exit(0);
			//::ExitProcess(NULL);
		}
	}

	return bRet;
}

