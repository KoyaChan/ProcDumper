#pragma once
#include "Logger.h"
#include <vector>

class ThisProcess
{
public:
	ThisProcess(const Logger* logger);
	~ThisProcess();
	void SetLogger(const Logger* logger);
	int EnablePrivilege(const LPTSTR lpszPrivilege);
	int Init();
private:
	int DeInit();
	int SetPrivilegeAttributes(const LUID luid, const DWORD dwAttributes);
	int SetPrivilegeAttributes(const LPTSTR lpszPrivilege, const DWORD dwAttributes);

private:
	HANDLE m_hToken;
	PTOKEN_PRIVILEGES m_OriginalPrivileges;
	int m_bPrivilegeChanged;
	const Logger* m_Logger;
};
