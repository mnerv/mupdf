/* strings.h — POSIX compatibility shim for Windows/MSVC.
 * Maps strcasecmp/strncasecmp to the MSVC equivalents.
 * On GCC/Clang: #include_next forwards to the real system strings.h so
 * this shim is safe even when it ends up on the include path inadvertently. */
#pragma once
#include <string.h>
#if defined(_WIN32)
#  define strcasecmp(a,b)    _stricmp((a),(b))
#  define strncasecmp(a,b,n) _strnicmp((a),(b),(n))
#else
#  include_next <strings.h>
#endif
