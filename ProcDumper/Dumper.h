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
	Dumper(const Logger* logger);
	~Dumper();
public:
	int GenerateDumpName();
	int Dump(const wchar_t* szDumpPath);
	void SetTarget(const TargetProcess* target);
	void SetLogger(const Logger* pLogger);
	wstring GetDumpName() const;

private:
	const Logger* m_Logger;
	wstring m_DumpName;
	const TargetProcess* m_TargetProcess;
	HANDLE m_hDumpFile;
};

