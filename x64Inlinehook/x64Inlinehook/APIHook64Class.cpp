#include "stdafx.h"
#include "APIHook64Class.h"

APIHook64::APIHook64()
{
	/*
	mov eax,0x12345678
	push eax
	ret
	*/
	unsigned char c[12] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xC3 };
	RtlMoveMemory(APIHook64::code, c, 12);
	memset(APIHook64::oldcode, 0, 12);
	addr = NULL;
}

BOOL APIHook64::Hook(char *dllName, char *apiName, long long callfunc, BOOL bHook)
{
	BOOL bOk = FALSE;
	DWORD dwOldProtect = 0;
	long long api = callfunc;
	HANDLE hPro = GetCurrentProcess();

	if (!APIHook64::oldcode[0])
	{
		addr = GetProcAddress(LoadLibraryA(dllName), apiName);
		RtlMoveMemory(APIHook64::code + 2, &api, 8);
		if (VirtualProtectEx(hPro, addr, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		{
			RtlMoveMemory(APIHook64::oldcode, addr, 12);
		}
	}
	if (bHook)
	{
		bOk = WriteProcessMemory(hPro, addr, APIHook64::code, 12, NULL);
	}
	else {
		bOk = WriteProcessMemory(hPro, addr, APIHook64::oldcode, 12, NULL);
	}
	VirtualProtectEx(hPro, addr, 12, dwOldProtect, &dwOldProtect);
	CloseHandle(hPro);
	return bOk;
}