#include "mercenaries.h"
#include "../auto_hook.h"
#include "../game/attributes.h"
#include "../game/player.h"
#include <cstdint>

// TODO use the correct ID
static const int16_t DisableKipchakTech = 1280;

void __thiscall hook_add_attribute(Player* player, int16_t attribute_id,
                                   float value, int32_t flag) {
  auto original =
      (void __thiscall (*)(Player*, int16_t, float, int32_t))0x45A990;
  original(player, attribute_id, value, flag);

  // When MercenaryKipchakCount reaches 0, trigger DisableKipchakTech
  if (attribute_id == static_cast<int16_t>(Attribute::MercenaryKipchakCount) &&
      player->attribute(Attribute::MercenaryKipchakCount) == 0.0f) {
    player->triggerTech(DisableKipchakTech);
  }
}

static VtblHook add_attribute_hook_;
void Mercenaries::install() {
  add_attribute_hook_.install((void*)0x637ABC, (void*)hook_add_attribute);
}
