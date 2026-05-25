# mb.toolib

[![build](https://github.com/devmarkusb/toolib/actions/workflows/build.yml/badge.svg)](https://github.com/devmarkusb/toolib/actions/workflows/build.yml)

A lib containing a wild collection of minimalistic tools to be shared
among apps. Criteria of what goes in:
* too general for any single app
* minimal or quick and dirty state, insufficient for an own
properly crafted lib
* not fundamental enough for util lib

## Usage with CMake

```
cmake_minimum_required(VERSION 3.30)

if(TARGET mb::toolib)
    return ()
endif ()

include(FetchContent)

FetchContent_Declare(mb-toolib
        GIT_REPOSITORY https://github.com/devmarkusb/toolib
        GIT_TAG origin/HEAD
        GIT_SUBMODULES_RECURSE ON
        GIT_SHALLOW  ON
        GIT_PROGRESS ON
        )

FetchContent_MakeAvailable(mb-toolib)
```

Include public headers from `mb/toolib`, for example:

```cpp
#include "mb/toolib/toolib.hpp"
```

Link the CMake target `mb::toolib`. The primary C++ namespace is `mb::too`; by default headers also expose the
shortcut alias `too::`, matching `ul::` from `mb.util`.

When configured as a top-level project, `toolib` fetches GoogleTest via `fetchcontent-lockfile.json` and falls back to
fetching `mb.util` if no installed `mb.util` package is available. CI checks out the `devenv` submodule recursively.
