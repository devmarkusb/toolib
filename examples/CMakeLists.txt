add_executable(mb.toolib.examples.usage)
target_sources(mb.toolib.examples.usage PRIVATE usage.cpp)
target_link_libraries(mb.toolib.examples.usage PRIVATE mb::toolib)
