#include "stdafx.h"
#include "ProcDumper.h"
#include "ThisProcess.h"


ThisProcess::ThisProcess()
	:m_bPrivilegeChanged(FALSE)
	,m_hToken(NULL)
	,m_OriginalPrivileges(NULL)
	,m_Logger(NULL)
{
	Init();
}


ThisProcess::~ThisProcess()
{
}


int ThisProcess::EnablePrivilege(const LPTSTR lpszPrivilege)
{
	return SetPrivilegeAttributes(lpszPrivilege, SE_PRIVILEGE_ENABLED);
}


void ThisProcess::SetLogger(Logger* logger)
{
	m_Logger = logger;
}


int ThisProcess::Init()
{
	if (!m_hToken)
	{
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &m_hToken))
		{
			m_Logger->Log(L"[%s] OpenProcessToken failed. ec=%d", __FUNCTIONW__, GetLastError());
			return PD_NG;
		}
	}

	if (!m_OriginalPrivileges)
	{
		DWORD dwLength = 0;
		GetTokenInformation(m_hToken, TokenPrivileges, NULL, 0, &dwLength);
		if (dwLength == 0)
		{
			m_Logger->Log(L"[%s] GetTokenInformation length is 0. ec=%d", GetLastError());
			return PD_NG;
		}

		m_OriginalPrivileges = (PTOKEN_PRIVILEGES)LocalAlloc(LPTR, dwLength);
		if (!GetTokenInformation(m_hToken, TokenPrivileges, m_OriginalPrivileges, dwLength, &dwLength))
		{
			m_Logger->Log(L"[%s] GetTokenInformation faied. ec=%d", GetLastError());
			return PD_NG;
		}
	}

	return PD_OK;
}


int ThisProcess::DeInit()
{
	if (!m_hToken)
	{
		return PD_OK;
	}
	
	if (m_OriginalPrivileges && m_bPrivilegeChanged)
	{
		DWORD PrivilegeCount = m_OriginalPrivileges->PrivilegeCount;
		for (DWORD i = 0; i < PrivilegeCount; i++)
		{
			LUID luid = m_OriginalPrivileges->Privileges[i].Luid;
			DWORD dwAttributes = m_OriginalPrivileges->Privileges[i].Attributes;
			if (SetPrivilegeAttributes(luid, dwAttributes) == PD_NG)
			{
				m_Logger->Log(L"[%s] AdjustTokenPrivileges failed. ec=%d", __FUNCTIONW__, GetLastError());
				return PD_NG;
			}
		}
		free(m_OriginalPrivileges);
	}

	CloseHandle(m_hToken);

	return PD_OK;
}


int ThisProcess::SetPrivilegeAttributes(const LUID luid, const DWORD dwAttributes)
{
	TOKEN_PRIVILEGES tokenPrivileges = { 0 };

	tokenPrivileges.PrivilegeCount = 1;
	tokenPrivileges.Privileges[0].Luid = luid;
	tokenPrivileges.Privileges[0].Attributes = dwAttributes;

	errno_t ec = ERROR_SUCCESS;
	if (!AdjustTokenPrivileges(m_hToken, FALSE, &tokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	{
		return PD_NG;
	}
	else if ((ec = GetLastError()) != ERROR_SUCCESS)
	{
		SetLastError(ec);
		return PD_NG;
	}

	m_bPrivilegeChanged = TRUE;
	return PD_OK;
}

int ThisProcess::SetPrivilegeAttributes(const LPTSTR lpszPrivilege, const DWORD dwAttributes)
{
	LUID   luid;
	TOKEN_PRIVILEGES tokenPrivileges;

	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
	{
		m_Logger->Log(L"[%s] LookupPrivilegeValue failed. ec=%d", __FUNCTIONW__, GetLastError());
		return PD_NG;
	}

	if (SetPrivilegeAttributes(luid, dwAttributes) == PD_NG)
	{
		m_Logger->Log(L"[%s] AdjustTokenPrivileges failed. privilege[%s] attribute[%lx] ec=%d", __FUNCTIONW__,
			lpszPrivilege, dwAttributes, GetLastError());
		return PD_NG;
	}

	return PD_OK;
}
