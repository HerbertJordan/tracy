package tracy

import "github.com/HerbertJordan/tracy/internal"

func StartupProfiler() {
	internal.StartupProfiler()
}

func ShutdownProfiler() {
	internal.ShutdownProfiler()
}

func FrameMark() {
	internal.FrameMark()
}

type Zone = internal.Zone

func ZoneBegin(name string) Zone {
	return internal.ZoneBegin(name)
}
