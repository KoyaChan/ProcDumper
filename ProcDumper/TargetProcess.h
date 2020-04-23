#pragma once
#include "Logger.h"

class TargetProcess
{
public:
	TargetProcess();
	~TargetProcess();
	int Find(const DWORD dwPid);
	int Find(const wchar_t* szProcessName);
	int SetLogger(const Logger* pLogger);
private:
	const Logger* m_Logger;
};

