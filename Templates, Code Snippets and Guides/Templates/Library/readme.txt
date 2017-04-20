For building and testing the libary 'standalone' (not as subdirectoy within an existing app that
should be extended by this library), you should copy the following content into a blank CMakeLists.txt
file right next to the Library dir.

+++
# Some notes for the occasional CMake user.
#
# 1 Debug or Release or...
# start cmake with arg either
# -DCMAKE_BUILD_TYPE=Debug
# or
# -DCMAKE_BUILD_TYPE=Release
# Other options are: MinSizeRel, RelWithDebInfo
# *Important notice* CMAKE_BUILD_TYPE cannot properly be used to query the current build type,
# at least not in a naive way, at least not for multiple build config. environments such as msvc.
#
# 2 Dynamic vs Static C/C++ runtime lib
# There is no support for static runtime libs! You could achieve this
# for MSVC, but this is not recommended. You or other will end up in
# conflicts soon.

cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(ToolsFL/tooBuildEnv)

include(${TOO_CMAKE_INC_DEFAULTS})

include(${TOO_CMAKE_INC_GOOGLETEST_AS_SUBDIR})

add_subdirectory(ToolsFL/tooLinkLib)
add_subdirectory(Library)
+++

And for that you will also need to clone the ToolsFL/tooBuildEnv and googletest repos
next to it. If the Library isn't header only, ToolsFL/tooLinkLib is helpful as well.
