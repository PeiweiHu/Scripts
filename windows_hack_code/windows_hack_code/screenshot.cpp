#include "stdafx.h"
#include "screenshot.h"


BOOL SaveBmp(HBITMAP hBmp)
{
	CImage image;

	// 附加位图句柄
	image.Attach(hBmp);

	// 保存成jpg格式图片
	image.Save("mybmp1.jpg");

	return TRUE;
}


BOOL screenshot() {
	// get desktop windows handle
	HWND hDesktopWnd = ::GetDesktopWindow();
	// get dc of desktop window
	HDC dcScreen = GetDC(hDesktopWnd);
	// get compatible dc
	HDC dcTarget = CreateCompatibleDC(dcScreen);
	// get size
	DWORD dwScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	DWORD dwScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	// create bitmap
	HBITMAP bmpTarget = CreateCompatibleBitmap(dcScreen, dwScreenWidth, dwScreenHeight);
	// select bitmap
	HGDIOBJ oldBmp = SelectObject(dcTarget, bmpTarget);
	// plot
	BitBlt(dcTarget, 0, 0, dwScreenWidth, dwScreenHeight, dcScreen, 0, 0, SRCCOPY);
	// save 
	SaveBmp(bmpTarget);
	// delete
	SelectObject(dcTarget, oldBmp);
	DeleteDC(dcTarget);
	DeleteDC(dcScreen);
	return TRUE;
}