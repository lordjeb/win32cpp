cmake_minimum_required(VERSION 3.9)
include("msvc.cmake")

project(win32cpp)
configure_msvc_runtime()

# Force the googletest to build the way we want it to (static or dynamic CRT)
if(${MSVC_RUNTIME} STREQUAL "static")
set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
else()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(/permissive-)

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(submodules)
