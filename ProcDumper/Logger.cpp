#include "stdafx.h"
#include "Logger.h"
#include <stdio.h>

Logger::Logger()
{
}


Logger::~Logger()
{
}


void Logger::SetLogPath(const wchar_t* szLogPath)
{
	m_LogPath = szLogPath;
}


void Logger::Log(const wchar_t* format, ...) const
{
	if (m_LogPath == L"")
	{
		return;
	}

	FILE *fp = NULL;
	errno_t err = _wfopen_s(&fp, m_LogPath.c_str(), L"a");
	if (err)
	{
		return;
	}

	wchar_t TempMsg[1024], TempMsg1[1024];
	va_list va;
	SYSTEMTIME systemTime;
	
	GetLocalTime(&systemTime);
	va_start(va, format);
	vswprintf_s(TempMsg1, 1024, format, va);
	va_end(va);

	swprintf_s(TempMsg, 1024, L"[%02d/%02d %02d:%02d.%03d] %s\n",
		systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond,
		TempMsg1);

	fwprintf_s(fp, L"%s", TempMsg);
	fflush(fp);
	fclose(fp);
}
