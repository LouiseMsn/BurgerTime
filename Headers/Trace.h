#pragma once

#ifdef _MSC_VER

#include <windows.h>
#include <atlbase.h>
#include <evntprov.h>
#include <strsafe.h>
#undef min
#undef max

void TraceRegister();
void TraceUnregister();
void TraceFormat(UCHAR Level, ULONGLONG Keyword, PCWSTR pszFormat, ...);
void TraceFormat(UCHAR Level, ULONGLONG Keyword, PCSTR pszFormat, ...);

#ifdef _DEBUG
#define TRACE(...) TraceFormat(0, 0, __VA_ARGS__);
#else
#define TRACE
#endif

#else

void TraceRegister();
void TraceUnregister();

#ifdef _DEBUG
#define TRACE(...) printf(__VA_ARGS__);
#else
#define TRACE
#endif

#endif // _MSC_VER
