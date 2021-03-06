file(GLOB header_list *.h)
file(GLOB source_list *.cpp)

add_definitions(-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
add_definitions("/analyze")
add_executable(lib_tests 
	${source_list}
	)

include_directories(${CMAKE_SOURCE_DIR}/src/lib)
include_directories(${gtest_SOURCE_DIR}/include ${gmock_SOURCE_DIR}/include ${COMMON_INCLUDES})
target_link_libraries(lib_tests win32cpp gtest gmock gmock_main)
if (MSVC_TOOLSET_VERSION GREATER_EQUAL 142)
	target_compile_features(lib_tests PRIVATE cxx_std_20)
elseif (MSVC_TOOLSET_VERSION GREATER_EQUAL 141)
	target_compile_features(lib_tests PRIVATE cxx_std_17)
else()
	target_compile_features(lib_tests PRIVATE cxx_std_14)
endif()
set_target_properties(lib_tests PROPERTIES FOLDER test)

# Disable C6326 which the gtest ASSERT_THAT/EXPECT_THAT macros trigger
target_precompile_headers(lib_tests PRIVATE <gmock/gmock.h> <Windows.h>)
set_target_properties(lib_tests PROPERTIES COMPILE_FLAGS "/wd6326")
