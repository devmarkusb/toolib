cmake_minimum_required(VERSION 3.30...4.3)

project(
    mb.toolib
    DESCRIPTION "toolib library"
    LANGUAGES CXX
    VERSION 0.1.0
)

option(
    MB_TOOLIB_BUILD_TESTS
    "Enable building toolib unit tests. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

option(
    MB_TOOLIB_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

option(
    MB_TOOLIB_DISABLE_NAMESPACE_ALIAS
    "Set to ON to disable the too:: shortcut alias for mb::too."
    OFF
)

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(devenv)
endif()
list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/devenv/cmake")

if(MB_TOOLIB_BUILD_TESTS)
    include(mb-devenv-ctest-root)
endif()
include(mb-devenv-defaults)
include(mb-devenv-install-library-config)
include("${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake")

# From fetched https://github.com/devmarkusb/pre-commit (see fetchcontent-lockfile.json).
mb_pre_commit_setup_project()

add_library(mb.toolib STATIC)
add_library(mb::toolib ALIAS mb.toolib)

target_compile_features(mb.toolib PUBLIC cxx_std_23)
target_compile_definitions(
    mb.toolib
    PUBLIC MB_TOOLIB_DISABLE_NAMESPACE_ALIAS=$<BOOL:${MB_TOOLIB_DISABLE_NAMESPACE_ALIAS}>
)
target_link_libraries(mb.toolib PUBLIC mb::util)

mb_devenv_set_target_filesystem(mb.toolib)
mb_devenv_set_target_defaults(mb.toolib)

set(_mb_toolib_fs_scope PUBLIC)

add_subdirectory(include/mb/toolib)
add_subdirectory(src)

if(NOT PROJECT_IS_TOP_LEVEL)
    set(MB_TOOLIB_INSTALL_CONFIG_FILE_PACKAGE OFF CACHE BOOL "" FORCE)
endif()
mb_devenv_install_library(mb.toolib)

if(MB_TOOLIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
