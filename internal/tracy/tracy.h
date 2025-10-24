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

go_tracy_Zone go_tracy_ZoneBegin(
    uint32_t line,
    const char *file,
    size_t file_len,
    const char *function,
    size_t function_len,
    const char* name,
    size_t name_len
);

uint32_t go_tracy_ZoneEnd(go_tracy_Zone zone);

#ifdef __cplusplus
}
#endif
