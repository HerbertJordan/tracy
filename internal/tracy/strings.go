//go:build enable_tracy

package tracy

import "C"
import "sync"

var stringCache map[string]*C.char = make(map[string]*C.char)
var stringCacheMutex sync.Mutex

// toCString converts a Go string to a C string, caching the result to avoid
// repeated allocations. Allocated C strings are never freed. Since those
// strings are used for profiling metadata that should comprise a finite set
// of file names, function names, and zone names, this should not lead to
// unbounded memory growth.
func toCString(text string) *C.char {
	stringCacheMutex.Lock()
	defer stringCacheMutex.Unlock()

	val, ok := stringCache[text]
	if ok {
		return val
	}
	cstr := C.CString(text)
	stringCache[text] = cstr
	return cstr
}
