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

include(defaults.cmake)

file(GLOB_RECURSE TargetToolib_INCS "include/*.h")
file(GLOB_RECURSE TargetToolib_SRC_INCS "src/*.h")

set(TargetToolib_SOURCES
    src/ToolibDummy.cpp
    src/dummy.cpp
    src/cons_w32/catch_exit.cpp
    src/cons_w32/cons_w32.cpp
    src/filesys/path.cpp
)

add_library(${TargetToolib} SHARED
	${TargetToolib_SOURCES}
	${TargetToolib_INCS}
	${TargetToolib_SRC_INCS}
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

target_include_directories(${TargetToolib} PUBLIC include)
target_include_directories(${TargetToolib} SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories(${TargetToolib} PUBLIC sdks/GSL/include)

set(Toolib_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set(Toolib_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)


################################################################################################

set(TargetToolibTest "ToolibTest")

file(GLOB_RECURSE TargetToolibTest_SRC_INCS "test/*.h")

set(TargetToolibTest_SOURCES
    test/src/assertTest.cpp
    test/src/finally.test.cpp
    test/src/narrow.test.cpp
    test/src/class/IAbstractTest.cpp
    test/src/math/numberTest.cpp
    test/src/math/roundTest.cpp
    test/src/math/scaleTest.cpp
    test/src/math/floating_pointTest.cpp
    test/src/filesys/pathTest.cpp
    test/src/performanceTest.cpp
    test/src/vector/vectorTest.cpp
)

add_executable(${TargetToolibTest}
	${TargetToolibTest_SOURCES}
	${TargetToolibTest_SRC_INCS}
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
