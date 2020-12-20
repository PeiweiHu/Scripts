# windows_hack_code proj

**single_instance** - run only one instnace (CreateMutex API).  

**free_resource** - release resource from resource section in PE.  

**inject_SetWindowsHookEx** - inject tech through SetWindowsHook, so far fail to run under my pc.     

**inject_CreateRemoteThread** - inject tech throgh CreateRemoteThread (works under both 32 bit and 64 bit).  

If you wanna inject a 64 bit process, both dll and program who executes injection should be 64 bit. Because 64 bit process can not run 32 bit dll, and 64 bit process's LoadLibrary start address is different from 32 bit process.  

**run_WinExec** - invoke some file through WinExec.  

**autorun_RegOpenKeyEx** - enable autorun after boot through changing HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run.  

**autorun_SHGetSpecialFolderPath** - enable autorun after boot through adding file to start up folder.  

**hide_NtQueryInfomationProcess** - hide process information through change peb. test under 64 bit os, trivial effect. Note that compiling program under 64 bit if os is 64 bit.  

**hide_ZwQuerySystemInformation** - hide process by inline hook (Fail to hide in Taskmgr.exe, error occurs).  

**dllhijack_WeChatWin_dll** - hijack WeChatWin.dll under wechat directory. Change original WeChatWin.dll to WeChatWinOrg.dll, then rename this new dll with WeChatWin.dll and move it to WeChat directory.  

**enum_process_CreateToolhelp32Snapshot** - list all process.  

**comm_socket_tcp** - communication between server and client. socket, tcp type.  

