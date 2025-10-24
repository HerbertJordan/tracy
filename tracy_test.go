package tracy

import (
	"sync"
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

func TestTraceSomeConcurrentZones(t *testing.T) {
	const N = 5
	StartupProfiler()
	defer ShutdownProfiler()

	zone := ZoneBegin("Main Zone")
	defer zone.End()

	var wg sync.WaitGroup
	wg.Add(N)
	for range N {
		go func() {
			defer wg.Done()
			zone := ZoneBegin("Concurrent Zone")
			time.Sleep(20 * time.Millisecond)
			zone.End()
		}()
	}
	wg.Wait()
}
