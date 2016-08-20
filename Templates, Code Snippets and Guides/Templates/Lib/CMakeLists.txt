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
project(LibProjName VERSION 0.1.0.${BUILD_NR})
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetLibName "LibName")

include(defaults.cmake)

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

if (WIN32)
set_property(TARGET ${TargetLibName} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${TargetLibName} APPEND PROPERTY COMPILE_DEFINITIONS LIBNAME_LIBRARY)

set_target_properties(${TargetLibName} PROPERTIES DEBUG_POSTFIX "d")

set(BOOST_ROOT $ENV{dev_sdk_paths})
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0)

configure_file(${TargetLibName}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetLibName}_build_config.h)
target_include_directories(${TargetLibName} PRIVATE ${CMAKE_BINARY_DIR})

target_include_directories(${TargetLibName} PRIVATE .)
target_include_directories(${TargetLibName} PRIVATE include)
target_include_directories(${TargetLibName} PRIVATE src)
target_include_directories(${TargetLibName} PRIVATE ${Boost_INCLUDE_DIRS})

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
set(LibName_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set(LibName_LIBRARY_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/lib PARENT_SCOPE)
endif()


################################################################################################

set(TargetLibNameTest "LibNameTest")

file(GLOB_RECURSE TargetLibNameTest_SRC_INCS "test/*.h*")

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

set(GTEST_ROOT $ENV{dev_sdk_paths})
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetLibNameTest} PRIVATE ${GTEST_INCLUDE_DIRS})
target_include_directories(${TargetLibNameTest} PRIVATE include)

target_link_libraries(${TargetLibNameTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetLibNameTest} PRIVATE ${TargetLibName})


add_custom_command(
  TARGET ${TargetLibNameTest} POST_BUILD
  COMMAND ${TargetLibNameTest}
)
