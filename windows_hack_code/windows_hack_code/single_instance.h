#pragma once

#include "stdafx.h"
// run single instance using CreateMutex

/*
	@brief: whether a mutex is not created yet
	@param: LPCTSTR
	@ret: 0 -> new mutex 1 -> already exist -1 -> fail to run
*/
INT firstRun(LPCTSTR name);
