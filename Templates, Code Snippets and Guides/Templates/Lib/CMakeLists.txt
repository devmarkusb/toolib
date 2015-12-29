# Some notes for the occasional CMake user.
#
# 1 Debug or Release or...
# start cmake with arg either
# -DCMAKE_BUILD_TYPE=Debug
# or
# -DCMAKE_BUILD_TYPE=Release
# Other options are: MinSizeRel, RelWithDebInfo
#
# 2 Dynamic vs Static C/C++ runtime lib
# There is no support for static runtime libs! You could achieve this
# for MSVC, but this is not recommended. You or other will end up in
# conflicts soon.

cmake_minimum_required(VERSION 3.4.1)

project(LibProjName VERSION 0.1.0.0)
set(TargetLibName "LibName")

set(OutputDir_bin ${CMAKE_SOURCE_DIR}/bin)
set(OutputDir_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OutputDir_bin})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OutputDir_lib})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OutputDir_lib})

# workaround for msvc, which adds additional subdirs per config
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
foreach (OutputConfig ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OutputConfig} OutputConfig)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_bin})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
endforeach ()
endif ()

# this disturbs linking to gtest, expects d there as well...
# so better handle this via set_target_properties
#set(CMAKE_DEBUG_POSTFIX "d")

# BITS will be 32, 64, ...
math(EXPR BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

# following 2 lines don't work as expected, why?
#set_property(GLOBAL PROPERTY CXX_STANDARD 11)
#set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-std=c++11")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/W4")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("-std=c++11")
    add_compile_options("-Wall")
endif ()

file(GLOB_RECURSE TargetLibName_INCS "include/*.h")
file(GLOB_RECURSE TargetLibName_SRC_INCS "src/*.h")

set(TargetLibName_SOURCES
    "src/non-empty folder.cpp"
)

add_library(${TargetLibName} SHARED
	${TargetLibName_SOURCES}
	${TargetLibName_INCS}
	${TargetLibName_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${TargetLibName} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${TargetLibName} APPEND PROPERTY COMPILE_DEFINITIONS LIBNAME_LIBRARY)

set_target_properties(${TargetLibName} PROPERTIES DEBUG_POSTFIX "d")

set(BOOST_ROOT "c:/projects/externlibs/boost_1_56_0")
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0)


target_include_directories(${TargetLibName} PRIVATE .)
target_include_directories(${TargetLibName} PRIVATE include)
target_include_directories(${TargetLibName} PRIVATE src)
target_include_directories(${TargetLibName} PRIVATE ${Boost_INCLUDE_DIRS})


################################################################################################

set(TargetLibNameTest "LibNameTest")

file(GLOB_RECURSE TargetLibNameTest_SRC_INCS "test/*.h")

set(TargetLibNameTest_SOURCES
	"test/non-empty folder.test.cpp"
)

add_executable(${TargetLibNameTest}
	${TargetLibNameTest_SOURCES}
	${TargetLibName_INCS}
	${TargetLibNameTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${TargetLibNameTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetLibNameTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetLibNameTest} PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TargetLibNameTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetLibNameTest} PRIVATE ${TargetLibName})


add_custom_command(
  TARGET ${TargetLibNameTest} POST_BUILD
  COMMAND ${TargetLibNameTest}
)
