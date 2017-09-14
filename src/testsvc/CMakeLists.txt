include_directories(${CMAKE_SOURCE_DIR}/src/lib)
add_executable(testsvc main.cpp)
target_link_libraries(testsvc win32cpp)
