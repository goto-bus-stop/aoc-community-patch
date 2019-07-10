#include "mercenaries.h"
#include "../auto_hook.h"
#include "../game/player.h"
#include <cstdint>

static const auto MERCENARY_KIPCHAK_COUNT = 214u;
static const auto MERCENARY_KIPCHAK_LIMIT = 215u;
// TODO use the correct ID
static const int16_t DISABLE_KIPCHACK_TECH = 1280;

void __thiscall hook_add_attribute(Player* player, int16_t attribute_id,
                                   float value, int32_t flag) {
  auto original =
      (void __thiscall (*)(Player*, int16_t, float, int32_t))0x45A990;
  original(player, attribute_id, value, flag);

  // When MERCENARY_KIPCHAK_COUNT reaches 0, trigger DISABLE_KIPCHACK_TECH
  if (attribute_id == MERCENARY_KIPCHAK_COUNT &&
      player->attribute(MERCENARY_KIPCHAK_COUNT) == 0.0f) {
    player->triggerTech(DISABLE_KIPCHACK_TECH);
  }
}

static VtblHook add_attribute_hook_;
void Mercenaries::install() {
  add_attribute_hook_.install((void*)0x637ABC, (void*)hook_add_attribute);
}
