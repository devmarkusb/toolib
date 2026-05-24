cmake_minimum_required(VERSION 3.30...4.3)

project(toolib VERSION 0.1.0)
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/devenv/cmake")
include(mb-devenv-defaults)
include(cmake_util/dependencies.cmake)
message(STATUS "${PROJECT_NAME} version: " ${PROJECT_VERSION})
set(target ${PROJECT_NAME})

file(GLOB_RECURSE target_INCS "include/*.h*")
file(GLOB_RECURSE target_SRC_INCS "src/*.h*")

set(target_SOURCES
    src/console/win/catch_exit.cpp
    src/filesys/path.cpp
    src/filesys/file_collection.cpp
    src/financial/money.cpp
)

add_library(${target} SHARED
    ${target_SOURCES}
    ${target_INCS}
    ${target_SRC_INCS}
)
target_compile_features(${target} PUBLIC cxx_std_23)

mb_devenv_set_target_lib_export_def(${target} TOOLIB_LIBRARY)

configure_file(${target}_build_config.h.in ${CMAKE_BINARY_DIR}/${target}_build_config.h)

target_include_directories(${target} PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(${target} PUBLIC include)

target_link_libraries(${target} PUBLIC mb::util)

mb_devenv_set_target_filesystem(${target})

mb_devenv_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${target}: include directories" FORCE)

################################################################################################

option(
    MB_TOOLIB_BUILD_TESTS
    "Enable building toolib unit tests. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

if (NOT MB_TOOLIB_BUILD_TESTS)
    return ()
endif ()

if(NOT TARGET GTest::gtest_main)
    find_package(GTest CONFIG REQUIRED)
endif()
include(GoogleTest)

set(targetTest ${PROJECT_NAME}-test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

target_include_directories(${targetTest} PRIVATE ${target}_INCLUDE_DIRS)

target_link_libraries(${targetTest} PRIVATE GTest::gtest_main ${target})

mb_devenv_set_target_defaults(${targetTest})

gtest_discover_tests(${targetTest})
