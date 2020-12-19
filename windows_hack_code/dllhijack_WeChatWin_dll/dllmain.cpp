// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#pragma comment(linker, "/EXPORT:??0IChannelLogWriter@@QAE@$$QAV0@@Z=WeChatWinOrg.??0IChannelLogWriter@@QAE@$$QAV0@@Z,@1")
#pragma comment(linker, "/EXPORT:??0IChannelLogWriter@@QAE@ABV0@@Z=WeChatWinOrg.??0IChannelLogWriter@@QAE@ABV0@@Z,@2")
#pragma comment(linker, "/EXPORT:??0IChannelLogWriter@@QAE@XZ=WeChatWinOrg.??0IChannelLogWriter@@QAE@XZ,@3")
#pragma comment(linker, "/EXPORT:??4IChannelLogWriter@@QAEAAV0@$$QAV0@@Z=WeChatWinOrg.??4IChannelLogWriter@@QAEAAV0@$$QAV0@@Z,@4")
#pragma comment(linker, "/EXPORT:??4IChannelLogWriter@@QAEAAV0@ABV0@@Z=WeChatWinOrg.??4IChannelLogWriter@@QAEAAV0@ABV0@@Z,@5")
#pragma comment(linker, "/EXPORT:??4ILogWriter@@QAEAAV0@$$QAV0@@Z=WeChatWinOrg.??4ILogWriter@@QAEAAV0@$$QAV0@@Z,@6")
#pragma comment(linker, "/EXPORT:??4ILogWriter@@QAEAAV0@ABV0@@Z=WeChatWinOrg.??4ILogWriter@@QAEAAV0@ABV0@@Z,@7")
#pragma comment(linker, "/EXPORT:??_7IChannelLogWriter@@6B@=WeChatWinOrg.??_7IChannelLogWriter@@6B@,@8")
#pragma comment(linker, "/EXPORT:?AddExtraMem@TXBugReport@@YAHKI@Z=WeChatWinOrg.?AddExtraMem@TXBugReport@@YAHKI@Z,@9")
#pragma comment(linker, "/EXPORT:?AddExtraMem@TXBugReport@@YAHPAXI@Z=WeChatWinOrg.?AddExtraMem@TXBugReport@@YAHPAXI@Z,@10")
#pragma comment(linker, "/EXPORT:?AddIgnoreHookCheckModule@TXBugReport@@YAXPB_W@Z=WeChatWinOrg.?AddIgnoreHookCheckModule@TXBugReport@@YAXPB_W@Z,@11")
#pragma comment(linker, "/EXPORT:?AddReleaseMonitorPoint@TXBugReport@@YAXPAJ@Z=WeChatWinOrg.?AddReleaseMonitorPoint@TXBugReport@@YAXPAJ@Z,@12")
#pragma comment(linker, "/EXPORT:?DoBugReport@TXBugReport@@YAJPAU_EXCEPTION_POINTERS@@PB_W@Z=WeChatWinOrg.?DoBugReport@TXBugReport@@YAJPAU_EXCEPTION_POINTERS@@PB_W@Z,@13")
#pragma comment(linker, "/EXPORT:?GetBugReportFlag@TXBugReport@@YAKXZ=WeChatWinOrg.?GetBugReportFlag@TXBugReport@@YAKXZ,@14")
#pragma comment(linker, "/EXPORT:?GetBugReportInfo@TXBugReport@@YAPAUtagBugReportInfo@1@XZ=WeChatWinOrg.?GetBugReportInfo@TXBugReport@@YAPAUtagBugReportInfo@1@XZ,@15")
#pragma comment(linker, "/EXPORT:?GetCustomFiltFunc@TXBugReport@@YAP6AHPAU_EXCEPTION_POINTERS@@@ZXZ=WeChatWinOrg.?GetCustomFiltFunc@TXBugReport@@YAP6AHPAU_EXCEPTION_POINTERS@@@ZXZ,@16")
#pragma comment(linker, "/EXPORT:?InitBugReport@TXBugReport@@YAXPB_W000GGKHHKKP6GHPAUtagBugReportInfo@1@PBD200PAPAXPAKPAX@Z@Z=WeChatWinOrg.?InitBugReport@TXBugReport@@YAXPB_W000GGKHHKKP6GHPAUtagBugReportInfo@1@PBD200PAPAXPAKPAX@Z@Z,@17")
#pragma comment(linker, "/EXPORT:?InitBugReportEx@TXBugReport@@YAXPB_W000GGKHHKKP6GHPAUtagBugReportInfo@1@PBD200PAPAXPAKPAX@ZH@Z=WeChatWinOrg.?InitBugReportEx@TXBugReport@@YAXPB_W000GGKHHKKP6GHPAUtagBugReportInfo@1@PBD200PAPAXPAKPAX@ZH@Z,@18")
#pragma comment(linker, "/EXPORT:?RaiseSelfFatalException@TXBugReport@@YAXW4SelfException@1@@Z=WeChatWinOrg.?RaiseSelfFatalException@TXBugReport@@YAXW4SelfException@1@@Z,@19")
#pragma comment(linker, "/EXPORT:?RecordCallStackIfNeed@TXBugReport@@YAXPAJ@Z=WeChatWinOrg.?RecordCallStackIfNeed@TXBugReport@@YAXPAJ@Z,@20")
#pragma comment(linker, "/EXPORT:?SetBugReportFlag@TXBugReport@@YAHK@Z=WeChatWinOrg.?SetBugReportFlag@TXBugReport@@YAHK@Z,@21")
#pragma comment(linker, "/EXPORT:?SetBugReportPath@TXBugReport@@YAHPB_W@Z=WeChatWinOrg.?SetBugReportPath@TXBugReport@@YAHPB_W@Z,@22")
#pragma comment(linker, "/EXPORT:?SetBugReportUin@TXBugReport@@YAXKH@Z=WeChatWinOrg.?SetBugReportUin@TXBugReport@@YAXKH@Z,@23")
#pragma comment(linker, "/EXPORT:?SetCustomFiltFunc@TXBugReport@@YAXP6AHPAU_EXCEPTION_POINTERS@@@Z@Z=WeChatWinOrg.?SetCustomFiltFunc@TXBugReport@@YAXP6AHPAU_EXCEPTION_POINTERS@@@Z@Z,@24")
#pragma comment(linker, "/EXPORT:?SetExtInfo@TXBugReport@@YAHKKPB_W@Z=WeChatWinOrg.?SetExtInfo@TXBugReport@@YAHKKPB_W@Z,@25")
#pragma comment(linker, "/EXPORT:?SetExtRptFilePath@TXBugReport@@YAHPB_W0@Z=WeChatWinOrg.?SetExtRptFilePath@TXBugReport@@YAHPB_W0@Z,@26")
#pragma comment(linker, "/EXPORT:?SetLogFileMd5Dir@TXBugReport@@YAHPB_W00@Z=WeChatWinOrg.?SetLogFileMd5Dir@TXBugReport@@YAHPB_W00@Z,@27")
#pragma comment(linker, "/EXPORT:?UninitBugReport@TXBugReport@@YAXXZ=WeChatWinOrg.?UninitBugReport@TXBugReport@@YAXXZ,@28")
#pragma comment(linker, "/EXPORT:?ValidateBugReport@TXBugReport@@YAXXZ=WeChatWinOrg.?ValidateBugReport@TXBugReport@@YAXXZ,@29")
#pragma comment(linker, "/EXPORT:?pfPostBugReport@TXBugReport@@3P6AXXZA=WeChatWinOrg.?pfPostBugReport@TXBugReport@@3P6AXXZA,@30")
#pragma comment(linker, "/EXPORT:?pfPreBugReport@TXBugReport@@3P6AXXZA=WeChatWinOrg.?pfPreBugReport@TXBugReport@@3P6AXXZA,@31")
#pragma comment(linker, "/EXPORT:SignWith3Des=WeChatWinOrg.SignWith3Des,@32")
#pragma comment(linker, "/EXPORT:StartWachat=WeChatWinOrg.StartWachat,@33")
#pragma comment(linker, "/EXPORT:_TlsGetData@12=WeChatWinOrg._TlsGetData@12,@34")
#pragma comment(linker, "/EXPORT:_TlsStoreData@12=WeChatWinOrg._TlsStoreData@12,@35")
#pragma comment(linker, "/EXPORT:__ASSERT=WeChatWinOrg.__ASSERT,@36")

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		::MessageBox(NULL, L"I am Peiwei", L"Hi", MB_OK);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

