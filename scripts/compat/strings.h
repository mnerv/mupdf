/* strings.h — POSIX compatibility shim for Windows/MSVC/Clang-on-Windows.
 * Provides strcasecmp / strncasecmp via the MSVC equivalents. */
#pragma once
#include <string.h>
#ifdef _WIN32
#  define strcasecmp(a,b)    _stricmp((a),(b))
#  define strncasecmp(a,b,n) _strnicmp((a),(b),(n))
#endif
