cmake_minimum_required(VERSION 3.8.2)

too_include(buildnr.cmake)
project(toolib VERSION 0.1.${BUILD_NR}.0)
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetToolib ${PROJECT_NAME})

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

too_set_target_lib_export_def(${TargetToolib} TOOLIB_LIBRARY)

configure_file(${TargetToolib}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetToolib}_build_config.h)

target_include_directories(${TargetToolib} PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(${TargetToolib} PUBLIC include)
target_include_directories(${TargetToolib} PUBLIC ${tfl_INCLUDE_DIRS})

target_link_libraries(${TargetToolib})

too_set_target_defaults(${TargetToolib})

set(${TargetToolib}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    CACHE INTERNAL "${TargetToolib}: include directories" FORCE)

    
################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

set(TargetToolibTest ${PROJECT_NAME}Test)

file(GLOB_RECURSE TargetToolibTest_SOURCES "include/toolib/*.test.cpp")

add_executable(${TargetToolibTest}
    ${TargetToolibTest_SOURCES}
    ${TargetToolib_INCS}
)

too_add_test(${TargetToolibTest})

target_include_directories(${TargetToolibTest} PRIVATE ${TargetToolib}_INCLUDE_DIRS)

target_link_libraries(${TargetToolibTest} PRIVATE gtest_main ${TargetToolib})

too_set_target_defaults(${TargetToolibTest})

too_run_target_postbuild(${TargetToolibTest})
