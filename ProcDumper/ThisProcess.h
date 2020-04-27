#pragma once
#include "Logger.h"
#include <vector>

class ThisProcess
{
public:
	ThisProcess();
	~ThisProcess();
	void SetLogger(Logger* logger);
	int EnablePrivilege(const LPTSTR lpszPrivilege);
private:
	HANDLE m_hToken;
	PTOKEN_PRIVILEGES m_OriginalPrivileges;
	int m_bPrivilegeChanged;
	Logger* m_Logger;
private:
	int Init();
	int DeInit();
	int SetPrivilegeAttributes(const LUID luid, const DWORD dwAttributes);
	int SetPrivilegeAttributes(const LPTSTR lpszPrivilege, const DWORD dwAttributes);
};

