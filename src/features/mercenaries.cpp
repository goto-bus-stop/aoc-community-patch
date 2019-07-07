#include "mercenaries.h"
#include "../auto_hook.h"
#include "../game/player.h"
#include <cstdint>

static const auto AVAILABLE_MERCENARIES_ATTRIBUTE = 214u;
static const auto USED_MERCENARIES_ATTRIBUTE = 215u;
static const int16_t DISABLE_MERCENARIES_TECH = 1280;

// When MAX_MERCENARIES_ATTRIBUTE reaches 0, trigger DISABLE_MERCENARIES_TECH

void __thiscall hook_add_attribute(Player* player, int16_t attribute_id,
                                   float value, int32_t flag) {
  auto original =
      (void __thiscall (*)(Player*, int16_t, float, int32_t))0x45A990;
  original(player, attribute_id, value, flag);

  if (attribute_id == AVAILABLE_MERCENARIES_ATTRIBUTE) {
    if (player->attribute(AVAILABLE_MERCENARIES_ATTRIBUTE) == 0) {
      player->triggerTech(DISABLE_MERCENARIES_TECH);
    }
  }
}

void Mercenaries::install() {}
