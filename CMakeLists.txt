cmake_minimum_required(VERSION 3.8.2)

include(${CMAKE_CURRENT_LIST_DIR}/cmake_util/util.cmake)

include(${ul_CMAKE_DIRS}/cmake_util.cmake)

ul_include(buildnr.cmake)
project(toolib VERSION 0.1.${BUILD_NR}.0)
message(STATUS "toolib project version: " ${PROJECT_VERSION})
set(TargetToolib ${PROJECT_NAME})

ul_include(defaults.cmake)

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

ul_set_target_lib_export_def(${TargetToolib} TOOLIB_LIBRARY)

configure_file(${TargetToolib}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetToolib}_build_config.h)

target_include_directories(${TargetToolib} PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(${TargetToolib} PUBLIC include)
target_include_directories(${TargetToolib} PUBLIC ${ul_INCLUDE_DIRS})

target_link_libraries(${TargetToolib})

ul_set_target_filesystem(${TargetToolib})

ul_set_target_defaults(${TargetToolib})

set(${TargetToolib}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${TargetToolib}: include directories" FORCE)

    
################################################################################################

if (NOT UL_BUILD_UNITTESTS)
    return ()
endif ()

set(TargetToolibTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE TargetToolibTest_SOURCES "include/toolib/*.test.cpp")

add_executable(${TargetToolibTest}
    ${TargetToolibTest_SOURCES}
    ${TargetToolib_INCS}
)

ul_add_test(${TargetToolibTest})

target_include_directories(${TargetToolibTest} PRIVATE ${TargetToolib}_INCLUDE_DIRS)

target_link_libraries(${TargetToolibTest} PRIVATE gtest_main ${TargetToolib})

ul_set_target_defaults(${TargetToolibTest})

ul_run_target_postbuild(${TargetToolibTest})
