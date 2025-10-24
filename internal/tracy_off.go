//go:build !enable_tracy

package internal

func StartupProfiler() {
	// No-op implementation when Tracy is disabled.
}

func ShutdownProfiler() {
	// No-op implementation when Tracy is disabled.
}

func FrameMark() {
	// No-op implementation when Tracy is disabled.
}

type Zone int

func ZoneBegin(name string) Zone {
	return Zone(0)
}

func (z Zone) End() {
}
