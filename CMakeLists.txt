cmake_minimum_required(VERSION 3.8.2)

include(${CMAKE_CURRENT_LIST_DIR}/cmake_util/util.cmake)

include(${ul_CMAKE_DIRS}/cmake_util.cmake)

ul_include(buildnr.cmake)
project(toolib VERSION 0.1.${BUILD_NR}.0)
message(STATUS "toolib project version: " ${PROJECT_VERSION})
set(target ${PROJECT_NAME})

ul_include(defaults.cmake)

ul_include(fetch-clangformat-repo.cmake)
ul_include(clang-format.cmake)
ul_include(githooks.cmake)

file(GLOB_RECURSE target_INCS "include/*.h*")
file(GLOB_RECURSE target_SRC_INCS "src/*.h*")

set(target_SOURCES
    src/console/win/catch_exit.cpp
    src/date_time/date_time.cpp
    src/filesys/path.cpp
    src/filesys/file_collection.cpp
    src/financial/money.cpp
)

add_library(${target} SHARED
    ${target_SOURCES}
    ${target_INCS}
    ${target_SRC_INCS}
)

ul_set_target_lib_export_def(${target} TOOLIB_LIBRARY)

configure_file(${target}_build_config.h.in ${CMAKE_BINARY_DIR}/${target}_build_config.h)

target_include_directories(${target} PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(${target} PUBLIC include)
target_include_directories(${target} PUBLIC ${ul_INCLUDE_DIRS})

target_link_libraries(${target})

ul_set_target_filesystem(${target})

ul_set_target_defaults(${target})

set(${target}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${target}: include directories" FORCE)

    
################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(targetTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE targetTest_SOURCES "src/*.test.cpp")

add_executable(${targetTest}
    ${targetTest_SOURCES}
    ${target_INCS}
)

ul_add_test(${targetTest})

target_include_directories(${targetTest} PRIVATE ${target}_INCLUDE_DIRS)

target_link_libraries(${targetTest} PRIVATE gtest_main ${target})

ul_set_target_defaults(${targetTest})

ul_run_target_postbuild(${targetTest})
