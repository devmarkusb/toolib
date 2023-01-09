# toolib library

A lib containing a wild collection of minimalistic tools to be shared
among apps. Criteria of what goes in:
* too general for any single app
* minimal or quick and dirty state, insufficient for an own
properly crafted lib
* not fundamental enough for util lib

## Usage with CMake

```
cmake_minimum_required(VERSION 3.14)

if (TARGET toolib)
    return ()
endif ()

include(FetchContent)

FetchContent_Declare(mb-toolib
        GIT_REPOSITORY https://github.com/devmarkusb/toolib
        GIT_TAG origin/HEAD
        GIT_SHALLOW ON
        )

FetchContent_MakeAvailable(mb-toolib)
```

In most cases you just need to include header files from subdir toolib,
CMake include dir: `${toolib_INCLUDE_DIRS}`.

Sometimes you need to link the lib, CMake target `toolib`.

To find anything topic specific, just browse through the directory
structure and read header files.

## FAQ

### Namespace clashes

In rare cases when you need to have another namespace `too`
around, you can set `UL_DISABLE_NAMESPACE_ALIAS` to `ON`
which yields base namespace `mb::too` instead.
