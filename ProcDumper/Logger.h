#pragma once
#include <string>
using namespace std;

class Logger
{
public:
	Logger(const wchar_t* szLogPath);
	~Logger();
	void Log(const wchar_t* format, ...) const;
	void SetLogPath(const wchar_t* szLogPath);
private:
	wstring m_LogPath;
};

