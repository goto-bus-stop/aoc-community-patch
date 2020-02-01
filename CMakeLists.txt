cmake_minimum_required(VERSION 3.9)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

if (UNIX)
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
endif()

project(community-patch)

if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
  set(IS_RELEASE TRUE)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ENABLE_LTO OUTPUT error)
  if (ENABLE_LTO)
    message(STATUS "Building with link time optimization")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  endif()
  if (NOT MSVC)
    add_link_options(-s)
  endif()
endif()

# Global defines
add_compile_definitions(WINVER=0x0501)
include_directories(include/)

add_library(community-patch SHARED)

target_sources(community-patch
  PRIVATE src/main.cpp
          src/config.cpp
          src/ini.c
          src/auto_hook.cpp
          src/features/attribute_storage_mode.cpp
          src/features/brb.cpp
          src/features/hill_bonus.cpp
          src/features/mercenaries.cpp
          src/features/num_idles.cpp
          src/features/num_resource_gatherers.cpp
          src/features/queueable_tech.cpp
          src/features/scx_mod_identifier.cpp
          src/fixes/keystate.cpp
          src/fixes/scenedit_minimap_position.cpp
          src/fixes/scenedit_pierce_armor.cpp
          src/game/panel.cpp
          src/game/player.cpp
          src/game/draw_area.cpp
)

target_compile_definitions(community-patch
  PRIVATE WIN32_LEAN_AND_MEAN
)

if(MSVC)
  foreach(flag_var
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
else()
  target_compile_options(community-patch
    PRIVATE -fno-exceptions
            -Wall -Wsign-compare
  )
  target_link_options(community-patch
    PRIVATE -static-libstdc++ -static-libgcc
  )
  if (IS_RELEASE)
    target_link_options(community-patch
      PRIVATE -Wl,--exclude-all-symbols
    )
  endif()
endif()

set_target_properties(community-patch PROPERTIES PREFIX "" SUFFIX ".dll")
