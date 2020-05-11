// ProcDumper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include "ProcDumper.h"
#include "TargetProcess.h"
#include "Dumper.h"
#include "Logger.h"
#include "ThisProcess.h"

int DumpProcess(const DWORD dwPid, const wchar_t* szProcessName, const wchar_t* szDumpPath)
{
	if (!dwPid && !szProcessName)
	{
		return PD_NG;
	}

	ThisProcess caller;
	TargetProcess target;
	Dumper dumper;
	Logger logger;

	// put the log file to the same path as dump
	wchar_t szLogPath[MAX_PATH] = { 0 };
	if (GenerateLogPath(szLogPath, MAX_PATH, szDumpPath) == PD_OK)
	{
		logger.SetLogPath(szLogPath);
		caller.SetLogger(&logger);
		target.SetLogger(&logger);
		dumper.SetLogger(&logger);
	}

	if (caller.Init() == PD_NG)
	{
		return PD_NG;
	}
	caller.EnablePrivilege((const LPTSTR)SE_DEBUG_NAME);
	caller.EnablePrivilege((const LPTSTR)SE_SECURITY_NAME);
	caller.EnablePrivilege((const LPTSTR)SE_TAKE_OWNERSHIP_NAME);

	// if dwPid is given, set the process of it to the target.
	// Otherwise, enum all pids of the given process name into the pids array,
	// and pick one of them and set the process of it to the target.
	DWORD pids[1024] = { 0 };
	DWORD count = 0;
	if (dwPid)
	{
		if (target.Find(dwPid) == PD_NG)
		{
			logger.Log(L"[%s] Pid %d not found", __FUNCTIONW__, dwPid);
			return PD_NG;
		}
		pids[0] = dwPid;
		count = 1;
	}
	else if (szProcessName)
	{
		if ((count = target.Find(szProcessName, pids, sizeof(pids) / sizeof(pids[0]))) == 0)
		{
			logger.Log(L"[%s] process %s not found", __FUNCTIONW__, szProcessName);
			return PD_NG;
		}
		else if (count > 0)
		{
			logger.Log(L"[%s] %d processes found", __FUNCTIONW__, count);
			logger.Log(L"[%s] target pid is [%d]", __FUNCTIONW__, target.GetPid());
		}
	}

	// dump the target process into a dump file located in szDumpPath
	dumper.SetTarget(&target);
	if (dumper.Dump(szDumpPath) == PD_NG)
	{
		logger.Log(L"[%s] Dump failed [%s]", __FUNCTIONW__, szDumpPath);
		return PD_NG;
	}

	return PD_OK;
}

// log file : "dumper.log" located in szDumpPath
int GenerateLogPath(wchar_t* szLogPath, const USHORT length, const wchar_t* szDumpPath)
{
	const wchar_t* szFileName = L"dumper.log";

	if (wcslen(szDumpPath) + wcslen(szFileName) + 1 > length)
	{
		return PD_NG;
	}

	swprintf_s(szLogPath, length, L"%s\\%s", szDumpPath, szFileName);

	return PD_OK;
}

// FindPid 
// In : process name
// Out : A pid of the process
// return : count of processes of the process name
int FindPid(const wchar_t* szProcessName, DWORD* dwPid, const wchar_t* szLogPath)
{
	TargetProcess target;
	Logger logger;

	// put the log file to the same path as dump
	wchar_t szLogFile[MAX_PATH] = { 0 };
	if (GenerateLogPath(szLogFile, MAX_PATH, szLogPath) == PD_OK)
	{
		logger.SetLogPath(szLogPath);
		target.SetLogger(&logger);
	}

	DWORD pids[1024] = { 0 };
	DWORD count = 0;
	if ((count = target.Find(szProcessName, pids, sizeof(pids) / sizeof(pids[0]))) == 0)
	{
		logger.Log(L"[%s] process %s not found", __FUNCTIONW__, szProcessName);
		*dwPid = 0;
		return 0;
	}
	else if (count > 0)
	{
		logger.Log(L"[%s] %d processes found", __FUNCTIONW__, count);
		*dwPid = target.GetPid();
		logger.Log(L"[%s] target pid is [%d]", __FUNCTIONW__, *dwPid);
	}

	return count;
}

