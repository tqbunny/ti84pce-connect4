#ifndef _DEBUG_H
#define _DEBUG_H

#include <debug.h>

#ifndef NDEBUG
#include <stdarg.h>

static void dprintf(const char* str, ...)
{
    va_list args;
    va_start(args, str);
    vsprintf(dbgout, str, args);
    va_end(args);
}

#else

#define dprintf //

#endif

#endif
