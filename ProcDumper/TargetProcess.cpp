#include "stdafx.h"
#include "TargetProcess.h"
#include <stdio.h>


TargetProcess::TargetProcess()
{
}


TargetProcess::~TargetProcess()
{
}


int TargetProcess::Find(DWORD dwPid)
{
	FILE *fp;
	errno_t err = fopen_s(&fp, "C:\\koyto02.txt", "a");
	fprintf(fp, "TargetProcess::Find called. pid: %d", dwPid);
	fclose(fp);
	// TODO: Add your implementation code here.
	return 0;
}


// szProcessName : executable file name of the process
int TargetProcess::Find(wchar_t* szProcessName)
{
	// TODO: Add your implementation code here.
	return 0;
}


// szDumpFile : Full pathname to the dump file
int TargetProcess::Dump(wchar_t* szDumpFile)
{
	// TODO: Add your implementation code here.
	return 0;
}
