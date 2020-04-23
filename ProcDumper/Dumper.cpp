#include "stdafx.h"
#include "Dumper.h"


Dumper::Dumper()
{
}


Dumper::~Dumper()
{
}

// generate dump file name into m_dumpname
// <process name>-<date>-<time>.dmp
int Dumper::GenerateDumpName()
{
	// TODO: Add your implementation code here.
	return 0;
}


// make the dump
int Dumper::Dump(const wchar_t* szDumpPath)
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
