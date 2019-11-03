#include "features/attribute_storage_mode.h"
#include "features/brb.h"
#include "features/hill_bonus.h"
#include "features/mercenaries.h"
#include "features/queueable_tech.h"
#include "features/scx_mod_identifier.h"
#include "fixes/keystate.h"
#include "fixes/scenedit_minimap_position.h"
#include <mmmod.h>
#include <windows.h>

extern "C" __declspec(dllexport) void mmm_load(mmm_mod_info* info) {
  info->name = "Community Patch (Core)";
  info->version = "0.0.0-alpha.0";
}

extern "C" __declspec(dllexport) void mmm_before_setup(mmm_mod_info* info) {
  // Fixes
  KeyState::install();
  ScenEditMinimapPosition::install();

  // Features
  AttributeStorageMode::install();
  BRB::install();
  HillBonus::install();
  // Buggy:
  if (false) Mercenaries::install();
  SCXModIdentifier::install();
  QueueableTech::install();

  FlushInstructionCache(GetCurrentProcess(), nullptr, 0);
}

extern "C" __declspec(dllexport) void mmm_unload(mmm_mod_info* info) {}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, void*) {
  if (reason == 1) {
    DisableThreadLibraryCalls(module);
  }
  return 1;
}
