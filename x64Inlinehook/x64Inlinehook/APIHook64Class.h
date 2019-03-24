#ifndef APIHOOK64CLASS_H_
#define APIHOOK64CLASS_H_
#include <Windows.h>

class APIHook64
{
private:
	unsigned char code[12];
	unsigned char oldcode[12];
	FARPROC addr;

public:
	APIHook64();
	BOOL Hook(char *dllName, char *apiName, long long callfunc, BOOL bHook = TRUE);
};

#endif