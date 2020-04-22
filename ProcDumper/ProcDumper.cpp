// ProcDumper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ProcDumper.h"
#include "TargetProcess.h"

//PROCDUMPER_API int DumpProcess(DWORD dwPid, wchar_t* szProcessName)
int DumpProcess(DWORD dwPid, wchar_t* szProcessName)
{
	TargetProcess targetProc;
	if (dwPid)
	{
		targetProc.Find(dwPid);
	}

	if (szProcessName)
	{
		targetProc.Find(szProcessName);
	}

	return 0;
}
