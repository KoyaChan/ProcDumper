#ifdef PROCDUMPER_EXPORTS
#define PROCDUMPER_API __declspec(dllexport)
#else
#define PROCDUMPER_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	PROCDUMPER_API int DumpProcess(DWORD dwPid, wchar_t* szProcessName);

#ifdef __cplusplus
}
#endif
