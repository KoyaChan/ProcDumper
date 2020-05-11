#ifdef PROCDUMPER_EXPORTS
#define PROCDUMPER_API __declspec(dllexport)
#else
#define PROCDUMPER_API __declspec(dllimport)
#endif

#define PD_OK 0
#define PD_NG -1

#ifdef __cplusplus
extern "C" {
#endif

	PROCDUMPER_API int DumpProcess(const DWORD dwPid, const wchar_t* szProcessName, const wchar_t* szDumpPath);
	PROCDUMPER_API int FindPid(const wchar_t* szProcessName, DWORD* dwPid, const wchar_t* szLogPath);

#ifdef __cplusplus
}
#endif

int GenerateLogPath(wchar_t* szLogPath, USHORT length, const wchar_t* szDumpPath);

