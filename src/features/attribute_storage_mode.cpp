#include "attribute_storage_mode.h"
#include "../game/unit.h"
#include "../game/player.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include <cstdint>

static const auto LATE_ATTR_FLAG = 16;

void take_late_attribute_from_owner(Unit* unit) {
  auto attributes = unit->type()->attributes();
  for (auto i = 0; i < 3; i++) {
    if (attributes.flag(i) == LATE_ATTR_FLAG) {
      auto t = attributes.type(i);
      auto amount = attributes.amount(i);
      if (t >= 0) {
        unit->owner()->addAttribute(t, -amount);
      }
    }
  }
}

void THISCALL(set_object_state, Unit* unit, uint8_t new_state) {
  auto original = getMethod<void, Unit*, uint8_t>(0x4D2480);
  if (new_state == 5) {
    // post-death animation complete
    take_late_attribute_from_owner(unit);
  }
  original(unit, new_state);
}

static VtblHook set_state_hook_;
void AttributeStorageMode::install() {
  set_state_hook_.install((void*)0x64842C, (void*)set_object_state);
}
