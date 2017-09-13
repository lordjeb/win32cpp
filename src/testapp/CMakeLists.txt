include_directories(${CMAKE_SOURCE_DIR}/src/lib)
add_executable(testapp main.cpp)
target_link_libraries(testapp win32cpp)