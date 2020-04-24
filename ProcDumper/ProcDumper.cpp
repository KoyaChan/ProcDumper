// ProcDumper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ProcDumper.h"
#include "TargetProcess.h"
#include "Dumper.h"
#include "Logger.h"

int DumpProcess(const DWORD dwPid, const wchar_t* szProcessName, const wchar_t* szDumpPath)
{
	if (!dwPid && !szProcessName)
	{
		return PD_NG;
	}

	TargetProcess target;
	Dumper dumper;
	Logger logger;

	wchar_t szLogPath[MAX_PATH] = { 0 };
	if (GenerateLogPath(szLogPath, MAX_PATH, szDumpPath) == PD_OK)
	{
		logger.SetLogPath(szLogPath);
		target.SetLogger(&logger);
		dumper.SetLogger(&logger);
	}

	if (dwPid)
	{
		if (target.Find(dwPid) == PD_NG)
		{
			logger.Log(L"[%s] Pid %d not found", __FUNCTIONW__, dwPid);
			return PD_NG;
		}
	}

	DWORD pids[1024] = { 0 };
	DWORD count = 0;
	if (szProcessName)
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

	dumper.SetTarget(&target);
	if (dumper.Dump(szDumpPath) == PD_NG)
	{
		logger.Log(L"[%s] Dump failed [%s]", __FUNCTIONW__, szDumpPath);
		return PD_NG;
	}

	logger.Log(L"[%s] Dump success [%s\\%s]", __FUNCTIONW__, szDumpPath, dumper.GetDumpName().c_str());

	return PD_OK;
}

int GenerateLogPath(wchar_t* szLogPath, USHORT length, const wchar_t* szDumpPath)
{
	const wchar_t* szFileName = L"dumper.log";

	if (wcslen(szDumpPath) + wcslen(szFileName) + 1 > length)
	{
		return PD_NG;
	}

	swprintf_s(szLogPath, length, L"%s\\%s", szDumpPath, szFileName);

	return PD_OK;
}

