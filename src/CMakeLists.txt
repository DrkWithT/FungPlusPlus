# src CMakeLists

add_executable(fungi fungi.cpp)

add_subdirectory(frontend)
# add_subdirectory(syntax)
# add_subdirectory(backend)

target_link_libraries(fungi PRIVATE frontend)
