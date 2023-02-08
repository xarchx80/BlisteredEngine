#include <PCH.h>
#include "Log.h"
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

char Log::filelineDummyStr[]{};

void Log::LogError(LPCSTR file, LPCSTR func, int line, LPCSTR format, ...)
{
	va_list args;
	va_start(args, format);
	//#to get filename without fullpath
	file = (strrchr(file, '\\') + 1);
	char buffer[256];
	char formatBuffer[128];
	vsnprintf(formatBuffer, -1, format, args);
	sprintf_s(buffer, "file : %s\nfunc : %s\nline : %d\n%s",file,func, line, formatBuffer);
	va_end(args);
	
	if (MessageBox(NULL, buffer, "Error", MB_OK | MB_ICONERROR)) {
		ExitProcess(0);
	}
}

void Log::LogLastError(LPCSTR file, int line, LPCSTR format, ...)
{
}

void Log::operator<<(HRESULT hr)
{
	if (FAILED(hr)) {
		TCHAR* buffer;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buffer, 0, NULL);
		LocalFree(&buffer);
		TCHAR totalBuffer[256];
		sprintf_s(totalBuffer, "%s\n%s\n", filelineDummyStr, buffer);
		if (MessageBox(NULL, totalBuffer, "HRESULT Error", MB_OK | MB_ICONQUESTION)) {
			ExitProcess(0);
		}
	}
}

Log& Log::operator()(LPCSTR filename, int line)
{
	memset(filelineDummyStr, 0, sizeof(filelineDummyStr));
	filename = (strrchr(filename, '\\') + 1);
	sprintf(filelineDummyStr,"file : %s\nline : %d\n", filename, line);
	return *this;
}


