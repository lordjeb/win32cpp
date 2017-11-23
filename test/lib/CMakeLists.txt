add_executable(lib_tests 
	handle_tests.cpp
	)

include_directories(${CMAKE_SOURCE_DIR}/src/lib)
include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include ${COMMON_INCLUDES})
target_link_libraries(lib_tests win32cpp gtest gmock gmock_main)
set_target_properties(lib_tests PROPERTIES FOLDER test)
