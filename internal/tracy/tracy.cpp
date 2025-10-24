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

typedef struct  ___tracy_source_location_data TracyCZoneLocation;


struct TracyZoneData {
    TracyZoneData* parent;
    TracyCZoneLocation loc;
    TracyCZoneCtx ctx;
};

// Active zones are tracked in a thread-local stack of TracyZoneData instances.
// The stack is used to implicitly close all open nested zones when closing an
// outer zone. While this should not happen in well-formed code, it is useful
// for recovering from instrumentation errors related to early returns or panics.
thread_local TracyZoneData* go_tracy_zones = nullptr;

go_tracy_Zone go_tracy_ZoneBegin(
    const char* name, 
    const char *function, 
    const char *file, 
    uint32_t line
) {
    // Grow stack of tracy zones.
    TracyZoneData* data = new TracyZoneData();
    data->parent = go_tracy_zones;
    go_tracy_zones = data;

    // Fill in zone data.
    data->ctx = TracyCZoneCtx {};
    data->loc = TracyCZoneLocation {};
    data->loc.name = name;
    data->loc.function = function;
    data->loc.file = file;
    data->loc.line = line;

    // Inform Tracy about the new zone.
    data->ctx = ___tracy_emit_zone_begin( (___tracy_source_location_data*)&data->loc, 1);
   
    return uintptr_t(data);
}

uint32_t go_tracy_ZoneEnd(go_tracy_Zone zone){
    // Pop zones off the stack until we reach the given zone.
    uint32_t numEnded = 0;
    while (go_tracy_zones != nullptr) {
        numEnded++;
         auto cur = go_tracy_zones;
        ___tracy_emit_zone_end(cur->ctx);
        go_tracy_zones = cur->parent;
        delete cur;
        if (uintptr_t(cur) == zone) {
            break;
        }
    }
    return numEnded;
}
