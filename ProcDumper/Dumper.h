#pragma once
#include "ProcDumper.h"
#include "TargetProcess.h"
#include "Logger.h"
#include <string>
using namespace std;

class Dumper
{
public:
	Dumper();
	~Dumper();
public:
	int GenerateDumpName();
	int Dump(const wchar_t* szDumpPath) const;
	void SetTarget(const TargetProcess* target);
	void SetLogger(const Logger* pLogger);

private:
	const Logger* m_Logger;
	wstring m_DumpName;
	const TargetProcess* m_TargetProcess;
public:
	wstring GetDumpName();
};

