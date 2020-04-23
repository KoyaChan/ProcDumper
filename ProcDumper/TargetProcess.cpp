#include "stdafx.h"
#include "ProcDumper.h"
#include "TargetProcess.h"
#include <stdio.h>


TargetProcess::TargetProcess()
{
}


TargetProcess::~TargetProcess()
{
}


int TargetProcess::Find(const DWORD dwPid)
{
	return PD_OK;
}


// szProcessName : executable file name of the process
int TargetProcess::Find(const wchar_t* szProcessName)
{
	// TODO: Add your implementation code here.
	return PD_OK;
}


int TargetProcess::SetLogger(const Logger* pLogger)
{
	m_Logger = pLogger;
	return PD_OK;
}
