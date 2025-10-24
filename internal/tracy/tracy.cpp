#include "tracy/TracyC.h"

#include "tracy.h"

void go_tracy_StartupProfiler() {
    ___tracy_startup_profiler();
}

void go_tracy_ShutdownProfiler() {
    ___tracy_shutdown_profiler();
}

void go_tracy_FrameMark() {
    ___tracy_emit_frame_mark((char*)0);
}


struct TracyZoneData {
    TracyZoneData* parent;
    TracyCZoneCtx ctx;
};

// Active zones are tracked in a thread-local stack of TracyZoneData instances.
// The stack is used to implicitly close all open nested zones when closing an
// outer zone. While this should not happen in well-formed code, it is useful
// for recovering from instrumentation errors related to early returns or panics.
thread_local TracyZoneData* go_tracy_zones = nullptr;

go_tracy_Zone go_tracy_ZoneBegin(
    uint32_t line,
    const char *file,
    size_t file_len,
    const char *function,
    size_t function_len,
    const char* name,
    size_t name_len
) {
    // Grow stack of tracy zones.
    TracyZoneData* data = new TracyZoneData();
    data->parent = go_tracy_zones;
    go_tracy_zones = data;

    // Create source location info.
    auto loc = ___tracy_alloc_srcloc_name(
        line,
        file,
        file_len,
        function,
        function_len,
        name,
        name_len,
        0
    );


    // Inform Tracy about the new zone.
    data->ctx = ___tracy_emit_zone_begin_alloc( loc, 1);

    return uintptr_t(data);
}

uint32_t go_tracy_ZoneEnd(go_tracy_Zone zone){
    // Pop zones off the stack until we reach the given zone.
    uint32_t numEnded = 0;
    while (go_tracy_zones != nullptr) {
        numEnded++;
        auto cur = go_tracy_zones;
        TracyCZoneEnd(cur->ctx);
        go_tracy_zones = cur->parent;
        delete cur;
        if (uintptr_t(cur) == zone) {
            break;
        }
    }
    return numEnded;
}
