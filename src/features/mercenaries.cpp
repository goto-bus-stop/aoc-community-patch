#include "mercenaries.h"
#include "../auto_hook.h"
#include "../game/player.h"
#include <cstdint>

static const auto AVAILABLE_MERCENARIES_ATTRIBUTE = 214u;
static const auto USED_MERCENARIES_ATTRIBUTE = 215u;
static const int16_t DISABLE_MERCENARIES_TECH = 1280;

void __thiscall hook_add_attribute(Player* player, int16_t attribute_id,
                                   float value, int32_t flag) {
  auto original =
      (void __thiscall (*)(Player*, int16_t, float, int32_t))0x45A990;
  original(player, attribute_id, value, flag);

  // When MAX_MERCENARIES_ATTRIBUTE reaches 0, trigger DISABLE_MERCENARIES_TECH
  if (attribute_id == AVAILABLE_MERCENARIES_ATTRIBUTE &&
      player->attribute(AVAILABLE_MERCENARIES_ATTRIBUTE) == 0.0f) {
    player->triggerTech(DISABLE_MERCENARIES_TECH);
  }
}

static VtblHook add_attribute_hook_;
void Mercenaries::install() {
  add_attribute_hook_.install((void*)0x637ABC, (void*)hook_add_attribute);
}
