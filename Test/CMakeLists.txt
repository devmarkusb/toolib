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

project(ToolibTest VERSION 0.1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

# this disturbs linking to gtest, expects d there as well...
#set(CMAKE_DEBUG_POSTFIX "d")

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

add_definitions(-DUNICODE -D_UNICODE)

add_executable(${PROJECT_NAME}
    Toolib/performanceTest.cpp
    Toolib/assertTest.cpp
    Toolib/vector/vectorTest.cpp
    Toolib/math/numberTest.cpp
    Toolib/math/roundTest.cpp
    Toolib/class/IAbstractTest.cpp
    Toolib/filesys/pathTest.cpp
    ../src/filesys/path.cpp
    Toolib/math/scaleTest.cpp
    Toolib/math/floating_pointTest.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

set(BOOST_ROOT "c:/projects/externlibs/boost_1_56_0")
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.56.0)

set(GTEST_ROOT "c:/projects/externlibs/gtest-1.7.0")
set(GTEST_MSVC_SEARCH   MD)
find_package(GTest 1.7.0)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIRS}) #${gtest_Path}/include)
include_directories(${Boost_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ../sdks/GSL/include)
target_include_directories(${PROJECT_NAME} PRIVATE ..)

target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
