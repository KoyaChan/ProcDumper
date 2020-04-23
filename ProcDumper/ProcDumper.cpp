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
			logger.Log(L"Pid %d not found", dwPid);
			return PD_NG;
		}
	}

	if (szProcessName)
	{
		if (target.Find(szProcessName) == PD_NG)
		{
			logger.Log(L"process %s not found", szProcessName);
			return PD_NG;
		}
	}

	dumper.SetTarget(&target);
	if (dumper.Dump(szDumpPath) == PD_NG)
	{
		logger.Log(L"Dump failed [%s]", szDumpPath);
		return PD_NG;
	}

	return PD_OK;
}

int GenerateLogPath(wchar_t* szLogPath, USHORT length, const wchar_t* szDumpPath)
{
	const wchar_t* szFileName = L"dumper.log";

	if (wcslen(szDumpPath) + wcslen(szFileName) > length - 1)
	{
		return PD_NG;
	}

	swprintf_s(szLogPath, length, L"%s\\%s", szDumpPath, szFileName);

	return PD_OK;
}

