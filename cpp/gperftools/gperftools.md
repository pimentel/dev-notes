# Google Perftools (gperftools)

This is a summary of info from the [gperftools
site](http://gperftools.googlecode.com/svn/trunk/doc/cpuprofile.html).

# Code to profile blocks

Make sure to include the header:

```cpp
#include <google/profiler.h>
```

You can then use the code `ProfilerStart("file.prof")` and `ProfilerStop()` to denote
blocks:

```cpp
ProfilerStart("file.prof");
// ...
std::cout << "This code will be profiled." << std::endl;
// ...
ProfilerStop();
```

<a name="linking"></a>

# Linking

The library should be in `*/lib/libprofiler.*`, thus, linking in `g++` looks
like:

```bash
g++ -o output_name -lprofiler source.cpp
```

# Running with the profiler

The previous section, [Linking](#linking) only adds the code to profile, and
doesn't enable profiling. To enable profiling, run a binary like this:

```bash
env CPUPROFILE=output.prof ./output_name
```

# Inspecting profiler output

You can inspect output using the tool `pprof`:

```bash
pprof --text ./output_name output.prof
# or, you can see the graph output
pprof --web ./output_name output.prof
```


