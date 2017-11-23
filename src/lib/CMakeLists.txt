set(source_list
    debug.cpp
    error.cpp
    module_info.cpp
    performance.cpp
    privilege_guard.cpp
    string_extensions.cpp
    debug.hpp
    error.hpp
    handle.hpp
    lock_guard.hpp
    map_view_deleter.hpp
    module_info.hpp
    performance.hpp
    privilege_guard.hpp
    ptr_deleter.hpp
    ptr_setter.hpp
    service.hpp
    string_extensions.hpp
    windows_constants.hpp
    ioc/handle.h
)

add_library(win32cpp STATIC
    ${source_list}
)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${source_list})
