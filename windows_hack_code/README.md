# windows_hack_code proj

**single_instance** - run only one instnace (CreateMutex API)  

**free_resource** - release resource from resource section in PE  

**inject_SetWindowsHookEx** - inject tech through SetWindowsHook, **so far fail to run under my pc**   

**inject_CreateRemoteThread** - inject tech throgh CreateRemoteThread. Note that when I use the 32 bit exe to inject a 32 bit target program with a 32 bit dll, it works. But I failed to do the same thing under 64 bit (including exe, target program and dll).  

**run_WinExec** - invoke some file through WinExec.  

**autorun_RegOpenKeyEx** - enable autorun after boot through changing HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run.  

**autorun_SHGetSpecialFolderPath** - enable autorun after boot through adding file to start up folder.  

**Note:** Since the whole project is under win32, you should find 32 bit program to inject. 

