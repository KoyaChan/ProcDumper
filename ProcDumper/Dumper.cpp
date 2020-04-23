#include "stdafx.h"
#include "Dumper.h"


Dumper::Dumper()
	:m_Logger(NULL)
	,m_DumpName(L"")
	,m_TargetProcess(NULL)
{
}

Dumper::~Dumper()
{
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

	m_DumpName += m_TargetProcess->GetProcessName() + L"-" + datetime;

	return PD_OK;
}


// make the dump
int Dumper::Dump(const wchar_t* szDumpPath) const
{
	// TODO: Add your implementation code here.
	return 0;
}


void Dumper::SetTarget(const TargetProcess* target)
{
	m_TargetProcess = target;
}


void Dumper::SetLogger(const Logger* pLogger)
{
	m_Logger = pLogger;
}


wstring Dumper::GetDumpName()
{
	return m_DumpName;
}
