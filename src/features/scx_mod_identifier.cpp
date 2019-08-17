#include "scx_mod_identifier.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/userpatch.h"

static CallHook on_save_scx_;
static UserPatch up;

static void THISCALL(on_save_hook, void* scx, void* handle) {
  printf("on_save_hook(%p, %p)\n", scx, handle);
  auto save = getMethod<void, void*, void*>(0x533180);
  auto set_player_name = getMethod<void, void*, uint32_t, const char*>(0x532940);
  auto get_player_name = getMethod<char*, void*, uint32_t>(0x5330F0);

  printf("set_player_name_before(%p, %p, %s)\n", scx, handle, up.modShortName());
  printf("%p\n", up.modShortName());
  set_player_name(scx, 9, up.modShortName());
  printf("set_player_name_after(%p, %p, %s)\n", scx, handle, get_player_name(scx, 9));
  save(scx, handle);
}

void SCXModIdentifier::install() {
  if (up.isModded()) {
    on_save_scx_.install((void*)0x4356ED, (void*)on_save_hook);
  }
}
