# Tracy

> ⚠️ This package location is temporary. Do not add dependencies.

Tracy is a Go library for instrumenting applications for the 
[Tracy](https://github.com/wolfpld/tracy) Frame Profiler. 

This library facilitates the instrumentation of multi-language applications
composed of multiple shared object files. To enable consistent tracing among
all of those, all bindings must link to a common shared Tracy Client library.

## Usage

This package can be included in any Go code by importing it using
```Go
import "github.com/HerbertJordan/tracy"
```
If you are implementing a binary, you must start-up the profiler manually using
```Go
tracy.StartupProfiler()
```
before passing any other instrumentation code. This will establish a connection
to an external Tracy profiler and report events. To shut down the connection at
the end of your application, call
```Go
tracy.ShutdownProfiler()
```

To instrument code zones, use the following code:
```Go
zone := tracy.BeginZone("myZoneLabel")
```
This automatically captures the current function name and code location in the
trace. To end the zone, use
```Go
zone.End()
```
The begin and end of a zone must be processed by the same go-routine. A typical
use case would combine these into
```Go
zone := tracy.BeginZone("myZoneLabel")
defer zone.End()
```
to cover the duration of a function.

## Building

By default, the instrumentation is disabled. To enable it, the `tracy` submodule
must be checked out and the build tag `enable_tracy` must be provided.
```bash
go build --tags=enable_tracy ./...
```
When enabled, the resulting executable requires access to shared library 
offering the Tracy Client functionality. To build the shared Tracy library, run
the following command
```bash
make tracy/build/libTracyClient.so
```

## Inspection
You can use the command
```bash
make tracy/profiler/build/tracy_profiler
```
to build Tracy's profiler tool and
```bash
./tracy/profiler/build/tracy_profiler
```
to run it to collect and visualize tracing data of instrumented code.
