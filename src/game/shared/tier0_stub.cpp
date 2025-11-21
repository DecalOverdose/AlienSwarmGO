#include "cbase.h"
#include "tier0/dbg.h"
#if defined(_WIN32) && defined(CLIENT_DLL)
#include <windows.h>
#ifndef CP_UTF8
#define CP_UTF8 65001
#endif
#include <direct.h>
#endif

// from dbg.h
void AssertValidStringPtr(const char* ptr, int maxchar/* = 0xFFFFFF */)
{
	// shit is stubbed
}