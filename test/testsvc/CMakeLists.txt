set (source_list
	main.cpp
)

ADD_MSVC_PRECOMPILED_HEADER("pch.h" "pch.cpp" source_list)
add_executable(testsvc ${source_list})
include_directories(${CMAKE_SOURCE_DIR}/src/lib)
target_link_libraries(testsvc win32cpp)
set_target_properties(testsvc PROPERTIES FOLDER test)
