set(header_list
    debug.h
    error.h
    handle.h
    lock_guard.h
    map_view_deleter.h
    module_info.h
	pch.h
    performance.h
    privilege_guard.h
    ptr_deleter.h
    ptr_setter.h
    service.h
    string_extensions.h
    windows_constants.h
)

set(source_list
    debug.cpp
    error.cpp
    module_info.cpp
    performance.cpp
    privilege_guard.cpp
    string_extensions.cpp
)

ADD_MSVC_PRECOMPILED_HEADER("pch.h" "pch.cpp" source_list)
add_library(win32cpp STATIC
    ${CMAKE_SOURCE_DIR}/README.md
    ${source_list}
    ${header_list}
)

assign_source_group("Source Files" ${source_list})
assign_source_group("Header Files" ${header_list})
