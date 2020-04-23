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
	void Log(const wchar_t* format, ...);
	void SetLogPath(const wchar_t* szLogPath);
};

