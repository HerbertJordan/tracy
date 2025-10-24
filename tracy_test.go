package tracy

import (
	"testing"
	"time"
)

func TestStartAndStopProfiler(t *testing.T) {
	// The test passes if no panic occurs.
	StartupProfiler()
	ShutdownProfiler()
}

func TestTraceSomeFrames(t *testing.T) {
	// The test passes if no panic occurs.
	StartupProfiler()
	defer ShutdownProfiler()

	for range 10 {
		FrameMark()

		zone := ZoneBegin("Test Zone")
		time.Sleep(10 * time.Millisecond)
		zone.End()
	}
}
