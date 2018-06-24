cmake_minimum_required(VERSION 3.8.2)

include(${TOO_CMAKE_INC_BUILDNR})
project(Toolib VERSION 0.1.${BUILD_NR}.0)
message(STATUS "project version: " ${PROJECT_VERSION})
set(TargetToolib ${PROJECT_NAME})

include(${TOO_CMAKE_INC_DEFAULTS})

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

set_target_properties(${TargetToolib} PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (WIN32)
    set_property(TARGET ${TargetToolib} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

# dll export define
set_property(TARGET ${TargetToolib} APPEND PROPERTY COMPILE_DEFINITIONS TOOLIB_LIBRARY)

set_target_properties(${TargetToolib} PROPERTIES DEBUG_POSTFIX "d")

configure_file(${TargetToolib}_build_config.h.in ${CMAKE_BINARY_DIR}/${TargetToolib}_build_config.h)
target_include_directories(${TargetToolib} PRIVATE ${CMAKE_BINARY_DIR})

target_include_directories(${TargetToolib} PUBLIC include)
target_include_directories(${TargetToolib} PUBLIC ${tooBasicCodeSupport_INCLUDE_DIRS})
target_include_directories(${TargetToolib} PUBLIC ${tooLinklib_INCLUDE_DIRS})
target_include_directories(${TargetToolib} PUBLIC ${tooString_INCLUDE_DIRS})
target_include_directories(${TargetToolib} PUBLIC ${tooMem_INCLUDE_DIRS})

target_link_libraries(${TargetToolib}) # tooBasicCodeSupport tooLinklib tooString tooMem

set(${TargetToolib}_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${tooBasicCodeSupport_INCLUDE_DIRS}
    ${tooLinklib_INCLUDE_DIRS}
    ${tooString_INCLUDE_DIRS}
    ${tooMem_INCLUDE_DIRS}
    CACHE INTERNAL "${TargetToolib}: include directories" FORCE)

    
################################################################################################

if (NOT TOO_BUILD_UNITTESTS)
    return ()
endif ()

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

target_include_directories(${TargetToolibTest} PRIVATE ${Toolib_INCLUDE_DIRS})

target_link_libraries(${TargetToolibTest} PRIVATE gtest_main ${TargetToolib})


set(do_auto_test    ON)

if (do_auto_test)
    add_custom_command(
        TARGET ${TargetToolibTest} POST_BUILD
        COMMAND ${TargetToolibTest}
        )
endif ()
