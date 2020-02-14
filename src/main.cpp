#include "features/attribute_storage_mode.h"
#include "features/brb.h"
#include "features/hill_bonus.h"
#include "features/mercenaries.h"
#include "features/num_idles.h"
#include "features/queueable_tech.h"
#include "features/scx_mod_identifier.h"
#include "fixes/keystate.h"
#include "fixes/scenedit_minimap_position.h"
#include "fixes/scenedit_pierce_armor.h"
#include "game/player.h"
#include <mmmod.h>
#include <windows.h>

// Override new operator with game new operator.
void* operator new(unsigned int size) {
  // Call game new
  return ((void* (*)(unsigned int))0x6137B0)(size);
}

// Override delete operator with game delete operator.
void operator delete(void *ptr) noexcept {
  // Call game delete
  ((void(*)(void*))0x6137BE)(ptr);
}

// Override new operator with game new operator.
void* operator new[](unsigned int size) {
  // Call game new
  return ((void* (*)(unsigned int))0x6137B0)(size);
}

// Override delete operator with game delete operator.
void operator delete[](void *ptr) noexcept {
  // Call game delete
  ((void(*)(void*))0x6137BE)(ptr);
}

extern "C" __declspec(dllexport) void mmm_load(mmm_mod_info* info) {
  info->name = "Community Patch (Core)";
  info->version = "0.0.0-alpha.0";
}

extern "C" __declspec(dllexport) void mmm_before_setup(mmm_mod_info* info) {
  auto base_dir = info->meta->mod_base_dir;
  char config_file[260];
  sprintf(config_file, "%sconfig.ini", base_dir);
  Config::getInstance()->load(config_file);

  // Support
  Player::install();

  // Fixes
  KeyState::install();
  ScenEditMinimapPosition::install();
  ScenEditPierceArmor::install();

  // Features
  AttributeStorageMode::install();
  BRB::install();
  HillBonus::install();
  Mercenaries::install();
  SCXModIdentifier::install();
  QueueableTech::install();
  NumIdles::install();

  FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
}

extern "C" __declspec(dllexport) void mmm_unload(mmm_mod_info* info) {}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, void*) {
  if (reason == 1) {
    DisableThreadLibraryCalls(module);
  }
  return 1;
}
