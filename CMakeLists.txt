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

project(Toolib VERSION 0.1.0.0)
set(TargetToolib "Toolib")

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


add_library(${TargetToolib} SHARED
    src/ToolibDummy.cpp
    src/dummy.cpp
    src/cons_w32/catch_exit.cpp
    src/cons_w32/cons_w32.cpp
    src/filesys/path.cpp
)

if (WIN32)
set_property(TARGET ${TargetToolib} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${TargetToolib} APPEND PROPERTY COMPILE_DEFINITIONS TOOLIB_LIBRARY)

set_target_properties(${TargetToolib} PROPERTIES DEBUG_POSTFIX "d")

set(BOOST_ROOT "c:/projects/externlibs/boost_1_56_0")
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0)

target_include_directories(${TargetToolib} PUBLIC .)
target_include_directories(${TargetToolib} PUBLIC include)
target_include_directories(${TargetToolib} PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(${TargetToolib} PUBLIC sdks/GSL/include)


################################################################################################

set(TargetToolibTest "ToolibTest")

add_executable(${TargetToolibTest}
    Test/src/assertTest.cpp
    Test/src/finally.test.cpp
    Test/src/narrow.test.cpp
    Test/src/class/IAbstractTest.cpp
    Test/src/math/numberTest.cpp
    Test/src/math/roundTest.cpp
    Test/src/math/scaleTest.cpp
    Test/src/math/floating_pointTest.cpp
    Test/src/filesys/pathTest.cpp
    Test/src/performanceTest.cpp
    Test/src/vector/vectorTest.cpp
)

if (WIN32)
set_property(TARGET ${TargetToolibTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetToolibTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetToolibTest} PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TargetToolibTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetToolibTest} PRIVATE ${TargetToolib})


add_custom_command(
  TARGET ${TargetToolibTest} POST_BUILD
  COMMAND ${TargetToolibTest}
)
