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

project(AppProjName VERSION 0.1.0.0)
set(TargetAppName "AppName")

set(OutputDir_bin ${CMAKE_SOURCE_DIR}/bin)
set(OutputDir_lib ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OutputDir_bin})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OutputDir_lib})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OutputDir_lib})

# workaround for msvc, which adds additional subdirs per config
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
foreach (OutputConfig ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OutputConfig} OutputConfig)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_bin})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OutputConfig} ${OutputDir_lib})
endforeach ()
endif ()

# this disturbs linking to gtest, expects d there as well...
# so better handle this via set_target_properties
#set(CMAKE_DEBUG_POSTFIX "d")

# BITS will be 32, 64, ...
math(EXPR BITS "8 * ${CMAKE_SIZEOF_VOID_P}")

# following 2 lines don't work as expected, why?
#set_property(GLOBAL PROPERTY CXX_STANDARD 11)
#set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-std=c++11")
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/W4")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("-std=c++11")
    add_compile_options("-Wall")
endif ()

file(GLOB_RECURSE TargetAppName_SRC_INCS "src/*.h")

set(TargetAppName_SOURCES
    src/globals.cpp
    src/_gui/src_WinCmdLine/main.cpp
    src/_gui/interface/globals_gui.cpp
)

add_executable(${TargetAppName}
	${TargetAppName_SOURCES}
	${TargetAppName_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${TargetAppName} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppName} PROPERTIES DEBUG_POSTFIX "d")

set(BOOST_ROOT "c:/projects/externlibs/boost_1_56_0")
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0)


target_include_directories(${TargetAppName} PRIVATE .)
target_include_directories(${TargetAppName} PRIVATE src)
target_include_directories(${TargetAppName} PRIVATE ${Boost_INCLUDE_DIRS})


################################################################################################

set(TargetAppNameTest "AppNameTest")


set(TargetAppNameAsLibForTest "AppNameAsLibForTest")

add_library(${TargetAppNameAsLibForTest} STATIC
	${TargetAppName_SOURCES}
	${TargetAppName_SRC_INCS}
)

get_target_property(TargetAppNameIncludeDirs ${TargetAppName} INCLUDE_DIRECTORIES)
target_include_directories(${TargetAppNameAsLibForTest} PUBLIC ${TargetAppNameIncludeDirs})
get_target_property(TargetAppNameLinkLibs ${TargetAppName} INTERFACE_LINK_LIBRARIES)
if (TargetAppNameLinkLibs)
	target_link_libraries(${TargetAppNameAsLibForTest} PUBLIC ${TargetAppNameLinkLibs})
endif ()


file(GLOB_RECURSE TargetAppNameTest_SRC_INCS "test/*.h")

set(TargetAppNameTest_SOURCES
	"test/non-empty folder.test.cpp"
)

add_executable(${TargetAppNameTest}
	${TargetAppNameTest_SOURCES}
	${TargetAppNameTest_SRC_INCS}
)

if (WIN32)
set_property(TARGET ${TargetAppNameTest} APPEND PROPERTY COMPILE_DEFINITIONS UNICODE _UNICODE)
endif ()

set_target_properties(${TargetAppNameTest} PROPERTIES DEBUG_POSTFIX "d")

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${TargetAppNameTest} PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TargetAppNameTest} PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${TargetAppNameTest} PRIVATE ${TargetAppNameAsLibForTest})


add_custom_command(
  TARGET ${TargetAppNameTest} POST_BUILD
  COMMAND ${TargetAppNameTest}
)
