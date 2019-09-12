cmake_minimum_required(VERSION 3.9)

project(community-patch)

if (UNIX)
  set(CMAKE_CROSS_COMPILING 1)
  set(CMAKE_SYSTEM_NAME Windows)
  set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
  set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
  set(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
endif()

set(CMAKE_CXX_STANDARD 17)
set(IS_RELEASE FALSE)

include_directories(include/)

if (IS_RELEASE)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ENABLE_LTO OUTPUT error)
  if (NOT MSVC)
    add_link_options(-s)
  endif()
endif()

add_compile_definitions(WINVER=0x0501)
add_compile_definitions(WIN32_LEAN_AND_MEAN)

if(MSVC)
  set(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
  set(CMAKE_C_FLAGS_MINSIZEREL_INIT     "/MT /O1 /Ob1 /D NDEBUG")
  set(CMAKE_C_FLAGS_RELEASE_INIT        "/MT /O2 /Ob2 /D NDEBUG")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /O2 /Ob1 /D NDEBUG")

  set(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
  set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "/MT /O1 /Ob1 /D NDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE_INIT        "/MT /O2 /Ob2 /D NDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /O2 /Ob1 /D NDEBUG")
else()
  add_compile_options(-fno-exceptions)
  if (IS_RELEASE)
    add_link_options(-Wl,--exclude-all-symbols)
  endif()
  add_link_options(-static-libstdc++ -static-libgcc)
  add_definitions(-Wall -Wsign-compare)
endif()

set(SOURCE_FILES
  src/main.cpp
  src/auto_hook.cpp
  src/features/attribute_storage_mode.cpp
  src/features/brb.cpp
  src/features/hill_bonus.cpp
  src/features/mercenaries.cpp
  src/features/scx_mod_identifier.cpp
  src/fixes/keystate.cpp
  src/fixes/scenedit_minimap_position.cpp
)

add_library(community-patch SHARED ${SOURCE_FILES})
set_target_properties(community-patch PROPERTIES
    PREFIX ""
    SUFFIX ".dll")
if(ENABLE_LTO)
  message(STATUS "Building with link time optimization")
  set_property(TARGET community-patch PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
target_link_libraries(community-patch)
install(TARGETS community-patch
        LIBRARY DESTINATION lib)
