#include "Headers/Trace.h"
#ifdef _MSC_VER

REGHANDLE _traceHandle = 0;

// 964d4572-adb9-4f3a-8170-fcbecec27465
// use WpfTraceSpy tool to read ETW string traces: https://github.com/smourier/TraceSpy 
static GUID GUID_Trace = { 0x964d4572,0xadb9,0x4f3a,{0x81,0x70,0xfc,0xbe,0xce,0xc2,0x74,0x65} };

void TraceRegister()
{
	EventRegister(&GUID_Trace, NULL, NULL, &_traceHandle);
}

void TraceUnregister()
{
	REGHANDLE h = _traceHandle;
	if (h)
	{
		_traceHandle = NULL;
		EventUnregister(h);
	}
}

void TraceFormat(UCHAR level, ULONGLONG keyword, PCWSTR format, ...)
{
	if (!_traceHandle)
		return;

	WCHAR szTrace[0x1000];
	va_list args;
	va_start(args, format);
	StringCchPrintfW(szTrace, 9 + 1, L"%08X:", GetCurrentThreadId());
	StringCchVPrintfW(((LPWSTR)szTrace) + 9, ARRAYSIZE(szTrace) - 10, format, args);
	va_end(args);
	EventWriteString(_traceHandle, level, keyword, szTrace);
}

void TraceFormat(UCHAR level, ULONGLONG keyword, PCSTR format, ...)
{
	USES_CONVERSION;
	if (!_traceHandle)
		return;

	CHAR szTrace[0x1000];
	va_list args;
	va_start(args, format);
	StringCchPrintfA(szTrace, 9 + 1, "%08X:", GetCurrentThreadId());
	StringCchVPrintfA(((LPSTR)szTrace) + 9, ARRAYSIZE(szTrace) - 10, format, args);
	va_end(args);
	EventWriteString(_traceHandle, level, keyword, A2W(szTrace));
}
#else

void TraceRegister() {}
void TraceUnregister() {}

#endif // _MSC_VER