#pragma once
#include "Logger.h"
#include <vector>

class TargetProcess
{
public:
	TargetProcess();
	~TargetProcess();
	int Find(const DWORD dwPid);
	DWORD Find(const wchar_t* szProcessName, DWORD* pids, DWORD size);
	int SetLogger(const Logger* pLogger);

private:
	const Logger* m_Logger;
	// these are set when the process is opened
	DWORD m_dwPid;          // set by Open
	wstring m_processName;  // set by Find
	HANDLE m_hProcess;      // set by Open
	// -----

private:
	int Open(DWORD dwPid);
	void Close();
public:
	DWORD GetPid() const;
	wstring GetProcessName() const;
};

