#include "auto_tech.h"
#include "../auto_hook.h"

static VtblHook add_attribute_hook_;
static VtblHook new_attribute_hook_;
static AutoTech registry_;

static void THISCALL(add_attribute, Player* player, Attribute attribute_id, float value, int earned) {
  auto original = getMethod<void, Player*, Attribute, float, earned>(0x45A990);
  original(player, attribute_id, value, earned);

  registry_.onAttributeUpdate(player, attribute_id);
}

static void THISCALL(new_attribute, Player* player, Attribute attribute_id, float value) {
  auto original = getMethod<void, Player*, Attribute, float, earned>(0x45A880);
  original(player, attribute_id, value);

  registry_.onAttributeUpdate(player, attribute_id);
}

void AutoTech::setup() {
  for (auto& map : this->may_trigger_tech_) {
    map.reset();
  }
}

void AutoTech::onAttributeUpdate(Player* player, Attribute attribute_id) {
  auto list = this->may_trigger_tech_[player->id()][attribute_id];
  if (!list) return;

  for (auto& tech_id : list) {
    if (player->canAffordTech(tech_id)) {
      player->triggerTech(tech_id);
    }
  }
}

void AutoTech::install() {
  add_attribute_hook_.install((void*)0x637A44 + (30 * 4), (void*)add_attribute);
  new_attribute_hook_.install((void*)0x637A44 + (29 * 4), (void*)new_attribute);
}
