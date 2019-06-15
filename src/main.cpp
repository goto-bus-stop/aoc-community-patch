#include "fixes/keystate.h"
#include "fixes/scenedit_minimap_position.h"
#include "features/brb.h"
#include <mmmod.h>
#include <windows.h>

extern "C" __declspec(dllexport) void mmm_load(mmm_mod_info* info) {
  info->name = "Community Patch (Core)";
  info->version = "0.0.0-alpha.0";
}

extern "C" __declspec(dllexport) void mmm_before_setup(mmm_mod_info* info) {
  KeyState::install();
  ScenEditMinimapPosition::install();
  BRB::install();
}

extern "C" __declspec(dllexport) void mmm_unload(mmm_mod_info* info) {}

WINAPI BOOL DllMain(HINSTANCE module, DWORD reason, void*) {
  if (reason == 1) {
    DisableThreadLibraryCalls(module);
  }
  return 1;
}
