#pragma once
#include "ProcDumper.h"
#include "TargetProcess.h"
#include "Logger.h"
#include "dbghelp.h"
#include <string>
using namespace std;

typedef BOOL(WINAPI *FN_MINIDUMPWRITEDUMP)(
	IN HANDLE hProcess,
	IN DWORD ProcessId,
	IN HANDLE hFile,
	IN MINIDUMP_TYPE DumpType,
	IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
	IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
	IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
	);

class Dumper
{
public:
	Dumper();
	~Dumper();
public:
	int GenerateDumpName();
	int Dump(const wchar_t* szDumpPath);
	void SetTarget(const TargetProcess* target);
	void SetLogger(const Logger* pLogger);

private:
	const Logger* m_Logger;
	wstring m_DumpName;
	const TargetProcess* m_TargetProcess;
public:
	wstring GetDumpName() const;
private:
	HANDLE m_hDumpFile;
};

