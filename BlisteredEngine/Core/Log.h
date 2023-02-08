#pragma once

#define LOG_ERROR(x, ...) Log::GetInstance()->LogError(__FILE__, __PF__, __LINE__, x, __VA_ARGS__)
#define LOG_LASTERROR(x, ...) Log::GetInstance()->LogLastError(__FILE__, __LINE__, x, __VA_ARGS__)
#define LOG_HR			  Log()(__FILE__, __LINE__)

#include <Core\Singleton.h>

class Log : public Singleton<Log>
{
public:
	void LogError(LPCSTR file, LPCSTR func, int line, LPCSTR fmt, ...);
	void LogLastError(LPCSTR file, int line, LPCSTR format, ...);

	void operator<<(HRESULT hr);
	Log& operator()(LPCSTR filename, int line);

	static char filelineDummyStr[128];
	
};

