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

include(buildnr.cmake)
project(LibProjName VERSION 0.1.0.${BUILD_NR})
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetLibName ${PROJECT_NAME})

include(defaults.cmake)

add_subdirectory(sdks)

file(GLOB_RECURSE TargetLibName_INCS "include/*.h*")
file(GLOB_RECURSE TargetLibName_SRC_INCS "src/*.h*")

set(TargetLibName_SOURCES
    "src/non-empty folder.cpp"
)

add_library(${TargetLibName} SHARED
	${TargetLibName_SOURCES}
	${TargetLibName_INCS}
	${TargetLibName_SRC_INCS}
)
# Uncomment, if this will be a header-only lib, then also replace SHARED by STATIC above
# *and* remove linkage to ${TargetLibName} for the test exe down below.
#set_target_properties(${TargetLibName} PROPERTIES LINKER_LANGUAGE CXX)

set_target_properties(${TargetLibName} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (WIN32)
    set_property(TARGET ${TargetLibName} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${TargetLibName} APPEND PROPERTY COMPILE_DEFINITIONS LIBNAME_LIBRARY)

set_target_properties(${TargetLibName} PROPERTIES DEBUG_POSTFIX "d")

configure_file(${TargetLibName}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetLibName}_build_config.h)
target_include_directories(${TargetLibName} PRIVATE ${CMAKE_BINARY_DIR})

target_include_directories(${TargetLibName} PUBLIC include)
target_include_directories(${TargetLibName} PRIVATE src)

target_link_libraries(${TargetLibName})

set(${TargetLibName}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${TargetLibName}: include directories" FORCE)


################################################################################################

set(TargetLibNameTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE TargetLibNameTest_SOURCES "src/*.test.cpp")

add_executable(${TargetLibNameTest}
	${TargetLibNameTest_SOURCES}
	${TargetLibName_INCS}
)

if (WIN32)
set_property(TARGET ${TargetLibNameTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetLibNameTest} PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(${TargetLibNameTest} PRIVATE include)

target_link_libraries(${TargetLibNameTest} PRIVATE gtest_main ${TargetLibName})

set(do_auto_test	ON)

if (do_auto_test)
    add_custom_command(
        TARGET ${TargetLibNameTest} POST_BUILD
        COMMAND ${TargetLibNameTest}
        )
endif ()
