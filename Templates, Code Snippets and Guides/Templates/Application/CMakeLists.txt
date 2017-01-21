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
project(AppProjName VERSION 0.1.0.${BUILD_NR})
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetAppName "AppName")

include(defaults.cmake)

add_subdirectory(sdks)

file(GLOB_RECURSE TargetAppNameAsLibForTest_SRC_INCS "src/*.h*")

# Sources to be switched according to active GUI implementation.
set(TargetAppNameAsLibForTest_IMPL_SOURCES
    src/_gui/src_WinCmdLine/resource_string.cpp
)

set(TargetAppNameAsLibForTest_SOURCES
    src/_gui/interface/resource_string.cpp

   ${TargetAppNameAsLibForTest_IMPL_SOURCES}
)

set(TargetAppNameAsLibForTest "${TargetAppName}AsLibForTest")

add_library(${TargetAppNameAsLibForTest} STATIC
        ${TargetAppNameAsLibForTest_SOURCES}
        ${TargetAppNameAsLibForTest_SRC_INCS}
)

if (WIN32)
    set_property(TARGET ${TargetAppNameAsLibForTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppNameAsLibForTest} PROPERTIES DEBUG_POSTFIX "d")

set(BOOST_ROOT $ENV{dev_sdk_paths})
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0) # e.g. COMPONENTS date_time

configure_file(${TargetAppName}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetAppName}_build_config.h)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${CMAKE_BINARY_DIR})

target_include_directories(${TargetAppNameAsLibForTest} PUBLIC .)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC src)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC sdks/ToolsFL/tooBuildEnv/include)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC sdks/ToolsFL/tooBasicCodeSupport/include)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC sdks/ToolsFL/tooString/include)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC SYSTEM ${Boost_INCLUDE_DIRS})

target_link_libraries(${TargetAppNameAsLibForTest} PRIVATE ${Boost_LIBRARIES})

add_executable(${TargetAppName}
    src/_gui/src_WinCmdLine/main.cpp
)

if (WIN32)
    set_property(TARGET ${TargetAppName} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppName} PROPERTIES DEBUG_POSTFIX "d")

target_link_libraries(${TargetAppName} PRIVATE ${TargetAppNameAsLibForTest})



################################################################################################

set(TargetAppNameTest "${TargetAppName}Test")

set(TargetAppNameTest_IMPL_SOURCES
    src/_gui/src_WinCmdLine/resource_string.test.cpp
)

set(TargetAppNameTest_SOURCES
    src/_gui/interface/resource_ids.test.cpp
    src/_gui/interface/resource_string.test.cpp

    ${TargetAppNameTest_IMPL_SOURCES}
)

add_executable(${TargetAppNameTest}
	${TargetAppNameTest_SOURCES}
    ${TargetAppNameAsLibForTest_SRC_INCS}
)

if (WIN32)
    set_property(TARGET ${TargetAppNameTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppNameTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT $ENV{dev_sdk_paths_gtest})
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetAppNameTest} PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TargetAppNameTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetAppNameTest} PRIVATE ${TargetAppNameAsLibForTest})


add_custom_command(
  TARGET ${TargetAppNameTest} POST_BUILD
  COMMAND ${TargetAppNameTest}
)
