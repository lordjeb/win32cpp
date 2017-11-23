MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

function(assign_source_group prefix)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        string(CONCAT _source_rel "${prefix}" "/" "${_source_rel}")
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

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
    win32api_abstractions.h
    windows_constants.h
)

set(source_list
    debug.cpp
    error.cpp
    module_info.cpp
#	pch.cpp
    performance.cpp
    privilege_guard.cpp
    string_extensions.cpp
    win32api_abstractions.cpp
)

ADD_MSVC_PRECOMPILED_HEADER("pch.h" "pch.cpp" source_list)
add_library(win32cpp STATIC
    ${CMAKE_SOURCE_DIR}/README.md
    ${source_list}
    ${header_list}
)

assign_source_group("Source Files" ${source_list})
assign_source_group("Header Files" ${header_list})
