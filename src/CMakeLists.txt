file(GLOB_RECURSE sources CONFIGURE_DEPENDS "*.cpp")
list(FILTER sources EXCLUDE REGEX "\\.test\\.cpp$")
target_sources(mb.toolib PRIVATE ${sources})

include(mb-devenv-compile-check)
mb_devenv_add_compile_check(
    mb.toolib.compile-check
    PRIVATE mb::toolib
    HEADER "mb/toolib/toolib.hpp"
)

if(NOT MB_TOOLIB_BUILD_TESTS)
    return()
endif()

include(mb-devenv-googletest)
mb_devenv_add_test(mb.toolib.test PRIVATE mb::toolib)
