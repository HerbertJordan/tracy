//go:build enable_tracy

package tracy

// #cgo CPPFLAGS: -Wno-unused-result -DTRACY_ENABLE=1 -DTRACY_DELAYED_INIT=1 -DTRACY_MANUAL_LIFETIME=1 -I${SRCDIR}/../../tracy/public
// #cgo LDFLAGS: -lTracyClient -L${SRCDIR}/../../tracy/build -Wl,-rpath=${SRCDIR}/../../tracy/build
// #include "tracy.h"
import "C"

import (
	"runtime"
)

func StartupProfiler() {
	C.go_tracy_StartupProfiler()
}

func ShutdownProfiler() {
	C.go_tracy_ShutdownProfiler()
}

func FrameMark() {
	C.go_tracy_FrameMark()
}

type Zone C.uintptr_t

func ZoneBegin(name string) Zone {
	// Tracy requires the begin and end of a zone to be on the same OS thread.
	runtime.LockOSThread()

	pc, filename, line, _ := runtime.Caller(3)
	funcname := runtime.FuncForPC(pc).Name()

	return Zone(C.go_tracy_ZoneBegin(
		C.uint32_t(line),
		toCString(filename),
		C.size_t(len(filename)),
		toCString(funcname),
		C.size_t(len(funcname)),
		toCString(name),
		C.size_t(len(name)),
	))
}

func (z Zone) End() {
	ended := C.go_tracy_ZoneEnd(C.uintptr_t(z))
	for i := C.uint32_t(1); i < ended; i++ {
		runtime.UnlockOSThread()
	}
}
