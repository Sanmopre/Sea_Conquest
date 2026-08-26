#ifndef __P2DEFS_H__
#define __P2DEFS_H__

#include <stdio.h>
#include <cstring>

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned char uchar;

#ifdef _MSC_VER
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
#else
#include <cstdint>
typedef uint32_t uint32;
typedef uint64_t uint64;
#endif

#ifndef _MSC_VER
#include <cstdarg>

inline int vsprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, va_list args)
{
	return vsnprintf(buffer, sizeOfBuffer, format, args);
}

inline int sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int result = vsnprintf(buffer, sizeOfBuffer, format, args);
	va_end(args);
	return result;
}

inline int strcpy_s(char* dest, size_t destSize, const char* src)
{
	if (destSize == 0)
		return -1;
	strncpy(dest, src, destSize - 1);
	dest[destSize - 1] = '\0';
	return 0;
}

inline int strcat_s(char* dest, size_t destSize, const char* src)
{
	size_t used = strlen(dest);
	if (used >= destSize)
		return -1;
	strncpy(dest + used, src, destSize - used - 1);
	dest[destSize - 1] = '\0';
	return 0;
}

inline int strncpy_s(char* dest, size_t destSize, const char* src, size_t count)
{
	if (destSize == 0)
		return -1;
	size_t toCopy = (count < destSize - 1) ? count : destSize - 1;
	strncpy(dest, src, toCopy);
	dest[toCopy] = '\0';
	return 0;
}
#endif

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	32
#define MID_STR		255
#define HUGE_STR	8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

#endif