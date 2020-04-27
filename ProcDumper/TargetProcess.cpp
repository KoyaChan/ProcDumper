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


// Open the process of the specified pid, and set it to this object
int TargetProcess::Find(const DWORD dwPid)
{
	wchar_t szProcessName[MAX_PATH] = { 0 };

	if (!dwPid)
	{
		m_Logger->Log(L"[%s] pid is 0", __FUNCTIONW__);
		return PD_NG;
	}

	if (Open(dwPid) == PD_NG)
	{
		m_Logger->Log(L"[%s] OpenProcess failed. pid[%d]", __FUNCTIONW__, dwPid);
		return PD_NG;
	}

	if (!GetModuleFileNameEx(m_hProcess, NULL, szProcessName, MAX_PATH))
	{
		m_Logger->Log(L"[%s] GetModuleFileNameEx failed. pid[%d] ec[%d]", __FUNCTIONW__, dwPid, GetLastError());
		return PD_NG;
	}

	m_processName = wcsrchr(szProcessName, L'\\') + 1;

	return PD_OK;
}


// szProcessName : executable file name of the process
// pids : array to retun the list of pids of the process
// max_pids : max count of elements to be able to set in the pids array
// return : how many processes of the name found. return 0 if error
// One of processes with the speicifed process name is opened and set to this object
DWORD TargetProcess::Find(const wchar_t* szProcessName, DWORD* pids, DWORD max_pids)
{
	DWORD *allPids = NULL;
	DWORD count = 0;
	DWORD cProcesses = 0;

	// enum all processes running on the system
	if (EnumAllPids(&allPids, &cProcesses) == PD_NG)
	{
		return 0;
	}

	DWORD foundIdx = 0;
	DWORD i;
	for (i = 0; i < cProcesses; i++)
	{
		if (allPids[i] == 0)
		{
			continue;
		}

		Find(allPids[i]);    // open the process
		if (m_processName == szProcessName)
		{
			count++;
			if (foundIdx < max_pids)
			{
				pids[foundIdx++] = allPids[i];
			}
			else
			{
				m_Logger->Log(L"[%s] pid[%d] is spilled out", __FUNCTIONW__, allPids[i]);
			}
		}
		Close();           // Close the process
	}
	delete[] allPids;
	allPids = NULL;

	if (count == 0)
	{
		m_Logger->Log(L"[%s] Process[%s] not found", __FUNCTIONW__, szProcessName);
		return 0;
	}

	// count can be greater than foundIdx because some pids are spilled out

	for (i = 0; i < foundIdx; i++)
	{
		if (Find(pids[i]) == PD_OK)
		{
			m_Logger->Log(L"[%s] pid[%d] process[%s] opened.", __FUNCTIONW__, m_dwPid, m_processName.c_str());
			return count;
		}
		else
		{
			m_Logger->Log(L"[%s] pid[%d] process[%s] open failed.", __FUNCTIONW__, pids[i], szProcessName);
		}
	}

	m_Logger->Log(L"[%s] failed to open a process [%s]", __FUNCTIONW__, szProcessName);

	return 0;
}


// set an array of all pids into ppPids,
// set the number of elements in the array into count
// caller needs to free the array by delete[]
int TargetProcess::EnumAllPids(DWORD** ppPids, DWORD* count)
{
	DWORD max_enum_processes = 256;
	DWORD cbNeeded, cProcesses;
	DWORD* allPids = NULL;

	do {
		if (allPids)
		{
			delete[] allPids;
			allPids = NULL;
			max_enum_processes *= 2;
		}
		allPids = new DWORD[max_enum_processes];

		if (allPids == NULL)
		{
			m_Logger->Log(L"[%s] memory allocation failed for all pids", __FUNCTIONW__);
			return PD_NG;
		}

		if (!EnumProcesses(allPids, sizeof(allPids[0]) * max_enum_processes, &cbNeeded))
		{
			m_Logger->Log(L"[%s] EnumProcesses failed. ec=%d", __FUNCTIONW__, GetLastError());
			delete[] allPids;
			return PD_NG;
		}

		cProcesses = cbNeeded / sizeof(allPids[0]);
	} while (cProcesses == max_enum_processes);		// until all pids are enumerated

	*ppPids = allPids;
	*count = cProcesses;
	return PD_OK;
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

	DWORD dwAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | THREAD_ALL_ACCESS;
	if ((m_hProcess = OpenProcess(dwAccess, FALSE, dwPid)) == NULL)
	{
		m_Logger->Log(L"[%s] OpenProcess failed. pid[%d] ec=%d", __FUNCTIONW__, dwPid, GetLastError());
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


HANDLE TargetProcess::GetHandle() const
{
	return m_hProcess;
}
