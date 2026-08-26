#include "p2Log.h"
#include "p2Defs.h"
#include <cstdarg>

#ifdef _WIN32
#include <windows.h>
#endif

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
#ifdef _WIN32
	OutputDebugString(tmp_string2);
#else
	fputs(tmp_string2, stdout);
#endif
}