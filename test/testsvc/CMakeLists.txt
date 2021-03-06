file(GLOB header_list *.h)
file(GLOB source_list *.cpp)

add_definitions("/analyze")
add_executable(testsvc ${source_list})
include_directories(${CMAKE_SOURCE_DIR}/src/lib)
target_link_libraries(testsvc win32cpp)
set_target_properties(testsvc PROPERTIES FOLDER test)
