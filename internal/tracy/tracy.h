#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef uintptr_t go_tracy_Zone;

void go_tracy_StartupProfiler();
void go_tracy_ShutdownProfiler();

void go_tracy_FrameMark();

go_tracy_Zone go_tracy_ZoneBegin(const char* name, const char *function, const char *file, uint32_t line);
uint32_t go_tracy_ZoneEnd(go_tracy_Zone zone);

#ifdef __cplusplus
}
#endif
