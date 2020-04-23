#include "stdafx.h"
#include "ProcDumper.h"
#include "TargetProcess.h"
#include <stdio.h>
#include <Psapi.h>


TargetProcess::TargetProcess()
	:m_Logger(NULL)
	, m_dwPid(0)
	, m_processName(L"")
	, m_hProcess(NULL)
{
}


TargetProcess::~TargetProcess()
{
	Close();
}


int TargetProcess::Find(const DWORD dwPid)
{
	wchar_t szProcessName[MAX_PATH] = { 0 };

	if (!dwPid)
	{
		m_Logger->Log(L"[%s] pid is 0", __FUNCTION__);
		return PD_NG;
	}

	if (Open(dwPid) == PD_NG)
	{
		m_Logger->Log(L"[%s] OpenProcess failed. pid[%d] ec[%d]", __FUNCTION__, dwPid, GetLastError());
		return PD_NG;
	}

	if (!GetModuleFileNameEx(m_hProcess, NULL, szProcessName, MAX_PATH))
	{
		m_Logger->Log(L"[%s] GetModuleFileNameEx failed. pid[%d] ec[%d]", __FUNCTION__, dwPid, GetLastError());
		return PD_NG;
	}

	m_processName = szProcessName;

	return PD_OK;
}


// szProcessName : executable file name of the process
// pids : array to retun the list of pids of the process
// size : size of pids array
// return : how many processes found
// This function opens one of processes with the speicifed process name
DWORD TargetProcess::Find(const wchar_t* szProcessName, DWORD* pids, DWORD size)
{
	DWORD allPids[1024] = { 0 };
	DWORD cbNeeded, cProcesses;
	DWORD count = 0;

	if (!EnumProcesses(allPids, sizeof(allPids), &cbNeeded))
	{
		m_Logger->Log(L"[%s] EnumProcesses failed. ec=%d", __FUNCTION__, GetLastError());
		return 0;
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	DWORD foundIdx = 0;
	DWORD i;
	for (i = 0; i < cProcesses; i++)
	{
		Find(allPids[i]);    // open the process
		if (m_processName == szProcessName)
		{
			count++;
			pids[foundIdx++] = allPids[i];
		}
		Close();
	}

	if (count == 0)
	{
		m_Logger->Log(L"[%s] Process[%s] not found", __FUNCTION__, szProcessName);
		return 0;
	}

	for (i = 0; i < count; i++)
	{
		if (Find(pids[i]) == PD_OK)
		{
			m_Logger->Log(L"[%s] pid[%d] process[%s] opened.", __FUNCTION__, m_dwPid, m_processName.c_str());
			return count;
		}
		else
		{
			m_Logger->Log(L"[%s] pid[%d] process[%s] open failed. ec=%d", __FUNCTION__, pids[i], szProcessName);
		}
	}

	m_Logger->Log(L"[%s] failed to open all processes [%s]", __FUNCTION__, szProcessName);

	return 0;
}


int TargetProcess::SetLogger(const Logger* pLogger)
{
	m_Logger = pLogger;
	return PD_OK;
}

int TargetProcess::Open(DWORD dwPid)
{
	if (dwPid == 0)
	{
		m_Logger->Log(L"[%s] pid is 0");
		return PD_NG;
	}
	//DWORD dwAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_DUP_HANDLE | THREAD_ALL_ACCESS;
	DWORD dwAccess = GENERIC_ALL;
	if ((m_hProcess = OpenProcess(dwAccess, FALSE, dwPid)) == NULL)
	{
		m_Logger->Log(L"[%s] OpenProcess failed. ec=%d", __FUNCTION__, GetLastError());
		return PD_NG;
	}

	m_dwPid = dwPid;
	return PD_OK;
}


void TargetProcess::Close()
{
	if (m_hProcess)
	{
		CloseHandle(m_hProcess);
		m_hProcess = NULL;
	}
	m_dwPid = 0;
	m_processName = L"";
}


DWORD TargetProcess::GetPid() const
{
	return m_dwPid;
}


wstring TargetProcess::GetProcessName() const
{
	return m_processName;
}
