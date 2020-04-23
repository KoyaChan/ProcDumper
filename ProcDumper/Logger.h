#pragma once
#include <string>
using namespace std;

class Logger
{
public:
	Logger();
	~Logger();
private:
	wstring m_LogPath;
public:
	void Log(const wchar_t* format, ...) const;
	void SetLogPath(const wchar_t* szLogPath);
};

