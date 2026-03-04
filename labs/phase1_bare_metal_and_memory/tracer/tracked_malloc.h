#ifndef TRACKED_MALLOC_H
#define TRACKED_MALLOC_H

#include <stddef.h>

void *tracked_malloc(size_t size, const char *file, int line);
void tracked_free(void *ptr, const char *file, int line);
void report_leak(void);

#ifndef SYS_MALLOC // checks if the switch to the normal malloc specified 

#define malloc(s) (tracked_malloc(s, __FILE__, __LINE__))
#define free(s) (tracked_free(s, __FILE__, __LINE__))

#endif

#endif
