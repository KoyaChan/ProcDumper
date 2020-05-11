#include "stdafx.h"
#include "Dumper.h"


Dumper::Dumper(const Logger* logger)
	:m_Logger(NULL)
	,m_DumpName(L"")
	,m_TargetProcess(NULL)
	,m_hDumpFile(INVALID_HANDLE_VALUE)
{
	SetLogger(logger);
}

Dumper::~Dumper()
{
	if (m_hDumpFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDumpFile);
		m_hDumpFile = INVALID_HANDLE_VALUE;
	}
}

// generate dump file name into m_dumpname
// <process name>-<date>-<time>.dmp
int Dumper::GenerateDumpName()
{
	wchar_t datetime[13] = { 0 };
	SYSTEMTIME systemTime;

	GetLocalTime(&systemTime);
	if (swprintf_s(datetime, sizeof(datetime), L"%02d%02d-%02d%02d%03d",
		systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond) == -1)
	{
		m_Logger->Log(L"[%s] swprintf_s failed. ec= %d", __FUNCTIONW__, GetLastError());
		return PD_NG;
	}

	m_DumpName += m_TargetProcess->GetProcessName() + L"-" + datetime + L".dmp";

	return PD_OK;
}


// make the dump into szDumpPath. Use GenerateDumpName to make dump file name.
int Dumper::Dump(const wchar_t* szDumpPath)
{
	if (m_TargetProcess == NULL || m_TargetProcess->GetHandle()== NULL)
	{
		m_Logger->Log(L"[%s] target process hasn't been opened", __FUNCTIONW__);
		return PD_NG;
	}

	HMODULE hDbgHelp = LoadLibrary(TEXT("DBGHELP.DLL"));
	if (hDbgHelp == NULL)
	{
		m_Logger->Log(L"[%s] failed to load dbghelp.dll ec=%d", __FUNCTIONW__, GetLastError());
		return PD_NG;
	}

	FN_MINIDUMPWRITEDUMP fnMiniDumpWriteDump = (FN_MINIDUMPWRITEDUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if (fnMiniDumpWriteDump == NULL)
	{
		m_Logger->Log(L"[%s] failed to find MiniDumpWriteDump function ec=%d", __FUNCTIONW__, GetLastError());
		return PD_NG;
	}

	if (GenerateDumpName() == PD_NG)
	{
		m_Logger->Log(L"[%s] GenerateDumpName failed.", __FUNCTIONW__);
		return PD_NG;
	}
	wstring DumpFullPath = wstring(szDumpPath) + L"\\" + m_DumpName;

	m_hDumpFile = CreateFileW(DumpFullPath.c_str(), GENERIC_WRITE, 0, NULL,
									CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_hDumpFile == INVALID_HANDLE_VALUE)
	{
		m_Logger->Log(L"[%s] CreateFile dumpfile failed. ec=%d", __FUNCTIONW__, GetLastError());
		return PD_NG;
	}

	// dump full memory
	if (!fnMiniDumpWriteDump(m_TargetProcess->GetHandle(), m_TargetProcess->GetPid(), m_hDumpFile,
								MiniDumpWithFullMemory, NULL, NULL, NULL))
	{
		m_Logger->Log(L"[%s] MiniDumpWriteDump failed. ec=%d", __FUNCTIONW__, GetLastError() & 0xffff);
		return PD_NG;
	}

	m_Logger->Log(L"[%s] Dump success. path[%s]", __FUNCTIONW__, DumpFullPath.c_str());

	return PD_OK;
}


void Dumper::SetTarget(const TargetProcess* target)
{
	m_TargetProcess = target;
}


void Dumper::SetLogger(const Logger* pLogger)
{
	m_Logger = pLogger;
}


wstring Dumper::GetDumpName() const
{
	return m_DumpName;
}
