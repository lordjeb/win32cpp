set(source_list
	error_tests.cpp
	handle_tests.cpp
	memory_tests.cpp
	module_info_tests.cpp
	performance_tests.cpp
	string_extensions_tests.cpp
)

ADD_MSVC_PRECOMPILED_HEADER("pch.h" "pch.cpp" source_list)
add_executable(lib_tests 
	${source_list}
	)

include_directories(${CMAKE_SOURCE_DIR}/src/lib)
include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include ${COMMON_INCLUDES})
target_link_libraries(lib_tests win32cpp gtest gmock gmock_main)
set_target_properties(lib_tests PROPERTIES FOLDER test)
