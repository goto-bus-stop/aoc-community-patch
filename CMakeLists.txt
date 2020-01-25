cmake_minimum_required(VERSION 3.9)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_STANDARD 17)

if (UNIX)
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
endif()

project(community-patch)

if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
  set(IS_RELEASE TRUE)
endif()

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
  foreach(flag_var
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
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
  src/features/num_idles.cpp
  src/features/queueable_tech.cpp
  src/features/scx_mod_identifier.cpp
  src/fixes/keystate.cpp
  src/fixes/scenedit_minimap_position.cpp
  src/fixes/scenedit_pierce_armor.cpp
  src/game/player.cpp
  src/game/draw_area.cpp
)

add_library(community-patch SHARED ${SOURCE_FILES})
set_target_properties(community-patch PROPERTIES PREFIX "" SUFFIX ".dll")
if(ENABLE_LTO)
  message(STATUS "Building with link time optimization")
  set_property(TARGET community-patch PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
install(TARGETS community-patch
        LIBRARY DESTINATION lib)
