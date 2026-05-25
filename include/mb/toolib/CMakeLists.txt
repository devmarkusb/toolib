file(
    GLOB_RECURSE headers
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)
target_sources(
    mb.toolib
    ${_mb_toolib_fs_scope}
    FILE_SET HEADERS
    BASE_DIRS
    "${PROJECT_SOURCE_DIR}/include"
    FILES
    ${headers}
)
