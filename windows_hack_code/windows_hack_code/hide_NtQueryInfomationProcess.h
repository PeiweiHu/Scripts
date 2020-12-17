#pragma once

#include "stdafx.h"
#include "winternl.h"


INT hide_NtQueryInformationProcess(DWORD dwPid, wchar_t * cmd, wchar_t * path);