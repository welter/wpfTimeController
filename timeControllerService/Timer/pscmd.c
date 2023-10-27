#include "pscmd.h"


/* NTAPI ZwQueryInformationProcess */
typedef NTSTATUS(NTAPI* Typedef_ZwQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
//Typedef_ZwQueryInformationProcess pNTAPI_ZwQueryInformationProcess =
//(Typedef_ZwQueryInformationProcess)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "ZwQueryInformationProcess");


/*
    获取指定进程命令行字符串，失败返回 FALSE（Unicode Version）
*/
BOOL WINAPI GetProcessCommandLineW(HANDLE hProcess, LPCWSTR lpcBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesCopied)
{
    BOOL result = FALSE;
    if (pNTAPI_ZwQueryInformationProcess)
    {
        PROCESS_BASIC_INFORMATION BasicInfo; memset(&BasicInfo, NULL, sizeof(BasicInfo));
        PEB PebBaseInfo; memset(&PebBaseInfo, NULL, sizeof(PebBaseInfo));
        RTL_USER_PROCESS_PARAMETERS ProcessParameters; memset(&ProcessParameters, NULL, sizeof(ProcessParameters));
        if (pNTAPI_ZwQueryInformationProcess(hProcess, ProcessBasicInformation, &BasicInfo, sizeof(BasicInfo), NULL) == STATUS_SUCCESS)
        {
            if (ReadProcessMemory(hProcess, BasicInfo.PebBaseAddress, &PebBaseInfo, sizeof(PebBaseInfo), NULL)
                && ReadProcessMemory(hProcess, PebBaseInfo.ProcessParameters, &ProcessParameters, sizeof(ProcessParameters), NULL))
            {
                if (lpcBuffer && nSize >= ProcessParameters.CommandLine.Length + 2)
                    result = ReadProcessMemory(hProcess, ProcessParameters.CommandLine.Buffer, (LPVOID)lpcBuffer,
                        ProcessParameters.CommandLine.Length, lpNumberOfBytesCopied);
                else if (lpNumberOfBytesCopied) { *lpNumberOfBytesCopied = ProcessParameters.CommandLine.Length + 2; result = TRUE; }
            }
        }
    }
    return result;
}

/*
    获取指定进程命令行字符串，失败返回 FALSE（Ansi Version）
    --------
    GetProcessCommandLineA 是基于 GetProcessCommandLineW 的 Ansi 版本，应用程序应尽可能使用 GetProcessCommandLineW，而不是此 GetProcessCommandLineA
*/
BOOL WINAPI GetProcessCommandLineA(HANDLE hProcess, LPCSTR lpcBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesCopied)
{
    BOOL result = FALSE;
    SIZE_T nCommandLineSize = NULL;
    if (GetProcessCommandLineW(hProcess, NULL, NULL, &nCommandLineSize))
    {
        WCHAR* lpLocalBuffer = (WCHAR*)malloc(nCommandLineSize);
        if (lpLocalBuffer)
        {
            memset(lpLocalBuffer, NULL, nCommandLineSize);
            if (GetProcessCommandLineW(hProcess, lpLocalBuffer, nCommandLineSize, &nCommandLineSize))
            {
                INT iNumberOfBytes = WideCharToMultiByte(CP_ACP, NULL, lpLocalBuffer, nCommandLineSize, (LPSTR)lpcBuffer, nSize, NULL, NULL);
                if (lpNumberOfBytesCopied) *lpNumberOfBytesCopied = (!lpcBuffer || (nSize < (iNumberOfBytes + 1))) ? iNumberOfBytes + 1 : iNumberOfBytes;
                result = iNumberOfBytes > 0;
            }
            free(lpLocalBuffer);
        }
    }
    return result;
}


main{

}
