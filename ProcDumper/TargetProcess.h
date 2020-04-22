#pragma once
class TargetProcess
{
public:
	TargetProcess();
	~TargetProcess();
	int Find(DWORD dwPid);
	int Find(wchar_t* szProcessName);
	int Dump(wchar_t* szDumpFile);
};

