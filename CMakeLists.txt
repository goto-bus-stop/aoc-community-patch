cmake_minimum_required(VERSION 3.0)

project(community-patch)

set(CMAKE_CROSS_COMPILING 1)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
set(CMAKE_RC_COMPILER i686-w64-mingw32-windres)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/)

add_definitions(-DWIN32_LEAN_AND_MEAN)
add_link_options(-static-libstdc++ -static-libgcc)

if(NOT MSVC)
  add_definitions(-Wall -Wsign-compare)
endif()

set(SOURCE_FILES
  src/main.cpp
  src/hook.c
  src/fixes/scenedit_minimap_position.cpp
  src/fixes/keystate.cpp
  src/features/brb.cpp
)

add_library(community-patch SHARED ${SOURCE_FILES})
set_target_properties(community-patch PROPERTIES
    PREFIX ""
    SUFFIX ".dll")
target_link_libraries(community-patch)
install(TARGETS community-patch
        LIBRARY DESTINATION lib)
