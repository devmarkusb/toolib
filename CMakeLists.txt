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

include(buildnr.cmake)
project(Toolib VERSION 0.1.0.${BUILD_NR})
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetToolib "Toolib")

include(defaults.cmake)

file(GLOB_RECURSE TargetToolib_INCS "include/*.h*")
file(GLOB_RECURSE TargetToolib_SRC_INCS "src/*.h*")

set(TargetToolib_SOURCES
    src/console/win/catch_exit.cpp
    src/filesys/path.cpp
    src/filesys/file_collection.cpp
    src/date_time/date_time.cpp
    src/financial/money.cpp
)

add_library(${TargetToolib} SHARED
	${TargetToolib_SOURCES}
	${TargetToolib_INCS}
	${TargetToolib_SRC_INCS}
)

option(Toolib_USE_NO_DEPENDENCIES "No 3rd party dependencies" OFF)

if (WIN32)
set_property(TARGET ${TargetToolib} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${TargetToolib} APPEND PROPERTY COMPILE_DEFINITIONS TOOLIB_LIBRARY)

set_target_properties(${TargetToolib} PROPERTIES DEBUG_POSTFIX "d")

if (Toolib_USE_NO_DEPENDENCIES)
set_property(TARGET ${TargetToolib} APPEND PROPERTY COMPILE_DEFINITIONS TOO_NO_DEPENDENCIES)
endif ()

if (NOT Toolib_USE_NO_DEPENDENCIES)
if (NOT Boost_INCLUDE_DIRS)
set(BOOST_ROOT $ENV{dev_sdk_paths})
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0)
endif ()
endif ()

configure_file(${TargetToolib}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetToolib}_build_config.h)
target_include_directories(${TargetToolib} PRIVATE ${CMAKE_BINARY_DIR})

target_include_directories(${TargetToolib} PRIVATE include)
if (NOT Toolib_USE_NO_DEPENDENCIES)
target_include_directories(${TargetToolib} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
endif ()
target_include_directories(${TargetToolib} PRIVATE ../ToolsFL/tooBuildEnv/include)
target_include_directories(${TargetToolib} PRIVATE ../ToolsFL/tooBasicCodeSupport/include)
target_include_directories(${TargetToolib} PRIVATE ../ToolsFL/tooLinklib/include)
target_include_directories(${TargetToolib} PRIVATE ../ToolsFL/tooString/include)
target_include_directories(${TargetToolib} PRIVATE ../ToolsFL/tooMem/include)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(Toolib_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set(Toolib_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)
endif()


################################################################################################

set(TargetToolibTest "ToolibTest")

file(GLOB_RECURSE TargetToolibTest_SRC_INCS "test/*.h*")

set(TargetToolibTest_SOURCES
    test/class/IAbstractTest.cpp
    test/class/IType.test.cpp
    test/class/counted_class_objects.test.cpp
    test/console/prompt.test.cpp
    test/console/win/catch_exit.test.cpp
    test/CppCLI/mmtypeconv.test.cpp
    test/debug/debug_leaks.test.cpp
    test/game/game.test.cpp
    test/image/image.test.cpp
    test/scope/raii_p.test.cpp
    test/scope/reftovalue.test.cpp
    test/scope/scopeflagged.bak.test.cpp
    test/scope/scopeflagged.test.cpp
    test/scope/scopeguard.test.cpp
    test/string/lex_cast.test.cpp
    test/string/string_token.test.cpp
    test/string/string_trim.test.cpp
    test/math/stat/histogram.test.cpp
    test/math/numberTest.cpp
    test/math/roundTest.cpp
    test/math/scaleTest.cpp
    test/math/floating_pointTest.cpp
    test/math/bits.test.cpp
    test/math/geometry.test.cpp
    test/math/graph.test.cpp
    test/math/matrix.test.cpp
    test/math/pair.test.cpp
    test/math/percent.test.cpp
    test/math/quantity_unit.test.cpp
    test/math/ratio.test.cpp
    test/math/standaloneop.test.cpp
    test/math/consts.test.cpp
    test/date_time/date_time.test.cpp
    test/date_time/cal_event.test.cpp
    test/filesys/pathTest.cpp
    test/filesys/file_collection.test.cpp
    test/performanceTest.cpp
    test/financial/money.test.cpp
)

add_executable(${TargetToolibTest}
	${TargetToolibTest_SOURCES}
	${TargetToolibTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${TargetToolibTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetToolibTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT $ENV{dev_sdk_paths})
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetToolibTest} PRIVATE ${GTEST_INCLUDE_DIRS})
if (NOT Toolib_USE_NO_DEPENDENCIES)
target_include_directories(${TargetToolibTest} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
endif ()
target_include_directories(${TargetToolibTest} PRIVATE include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooBuildEnv/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooBasicCodeSupport/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooLinklib/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooMem/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooString/include)

target_link_libraries(${TargetToolibTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetToolibTest} PRIVATE ${TargetToolib})


set(do_auto_test	ON)

if (do_auto_test)
add_custom_command(
  TARGET ${TargetToolibTest} POST_BUILD
  COMMAND ${TargetToolibTest}
)
endif ()
