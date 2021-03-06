file(GLOB header_list *.h)
file(GLOB source_list *.cpp)

set(CMAKE_DEBUG_POSTFIX  "")

add_definitions("/analyze")
add_library(win32cpp STATIC
    ${CMAKE_SOURCE_DIR}/README.md
    ${source_list}
    ${header_list}
)

source_group("Source Files" ${source_list})
source_group("Header Files" ${header_list})

target_precompile_headers(
    win32cpp 
    PRIVATE 
        <algorithm> 
        <memory> 
        <string> 
        <vector> 
        <Windows.h>
    )
