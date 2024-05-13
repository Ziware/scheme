#pragma once

#ifdef NDEBUG
#define ASSERT(condition, message) ((void)0)
#else
#define ASSERT(condition, message) \
    ((void)((condition) ? ((void)0) : __ASSERT(#condition, __FILE__, __LINE__, message)))
#define __ASSERT(cond, file, line, mess) \
    ((void)printf("%s:%d: failed assertion [%s]. %s\n", file, line, cond, mess))
#endif

#define CONTEXT(message) ((void)printf("%s:%d: %s", __FILE__, __LINE__, message))
