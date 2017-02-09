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
set(TargetToolib ${PROJECT_NAME})

include(defaults.cmake)

file(GLOB_RECURSE TargetToolib_INCS "include/*.h*")
file(GLOB_RECURSE TargetToolib_SRC_INCS "src/*.h*")

set(TargetToolib_SOURCES
    src/console/win/catch_exit.cpp
    src/date_time/date_time.cpp
    src/filesys/path.cpp
    src/filesys/file_collection.cpp
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

set_target_properties(${TargetToolib}
                      PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(${TargetToolib})

set(${TargetToolib}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${TargetToolib}: include directories" FORCE)

    
################################################################################################

set(TargetToolibTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE TargetToolibTest_SOURCES "include/Toolib/*.test.cpp")

add_executable(${TargetToolibTest}
    ${TargetToolibTest_SOURCES}
    ${TargetToolib_INCS}
)

if (WIN32)
set_property(TARGET ${TargetToolibTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetToolibTest} PROPERTIES DEBUG_POSTFIX "d")

if (NOT Toolib_USE_NO_DEPENDENCIES)
    target_include_directories(${TargetToolibTest} SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})
endif ()
target_include_directories(${TargetToolibTest} PRIVATE include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooBuildEnv/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooBasicCodeSupport/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooLinklib/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooMem/include)
target_include_directories(${TargetToolibTest} PRIVATE ../ToolsFL/tooString/include)

target_link_libraries(${TargetToolibTest} PRIVATE gtest_main)
target_link_libraries(${TargetToolibTest} PRIVATE ${TargetToolib})


set(do_auto_test	ON)

if (do_auto_test)
    add_custom_command(
        TARGET ${TargetToolibTest} POST_BUILD
        COMMAND ${TargetToolibTest}
        )
endif ()
