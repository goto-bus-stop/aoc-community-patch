#include "mercenaries.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/attributes.h"
#include "../game/player.h"
#include <cstdint>

static const int16_t DisableKipchakTech = 708;

static void check_kipchak(Player* player) {
  auto log_message = getMethod<int32_t, void*, const char*, const char*,
                               int32_t, int32_t, int32_t>(0x5E2780);

  // When MercenaryKipchakCount reaches 0, trigger DisableKipchakTech
  if (player->attribute(Attribute::MercenaryKipchakCount) == 0.0f) {
    player->triggerTech(DisableKipchakTech);
  }
}

void THISCALL(hook_add_attribute, Player* player, int16_t attribute_id,
                                   float value, int32_t flag) {
  auto old_value = attribute_id == static_cast<int16_t>(Attribute::MercenaryKipchakCount)
    ? player->attribute(Attribute::MercenaryKipchakCount)
    : NAN;

  auto original = getMethod<void, Player*, int16_t, float, int32_t>(0x45A990);
  original(player, attribute_id, value, flag);

  // When MercenaryKipchakCount reaches 0, trigger DisableKipchakTech
  if (attribute_id == static_cast<int16_t>(Attribute::MercenaryKipchakCount) && old_value > 0.0f) {
    check_kipchak(player);
  }
}

int8_t THISCALL(hook_pay_unit_price, Player* player, int16_t unit_id, float modifier, int32_t ignore_pop) {
  auto old_value = player->attribute(Attribute::MercenaryKipchakCount);

  auto original = getMethod<int32_t, Player*, int16_t, float, int32_t>(0x457E40);
  auto result = original(player, unit_id, modifier, ignore_pop);
  if (result && old_value > 0.0f) {
    check_kipchak(player);
  }

  return result;
}

static VtblHook add_attribute_hook_;
static CallHook pay_unit_price_hook_[2];
void Mercenaries::install() {
  add_attribute_hook_.install((void*)0x637ABC, (void*)hook_add_attribute);
  pay_unit_price_hook_[0].install((void*)0x4CB7DF, (void*)hook_pay_unit_price);
  pay_unit_price_hook_[1].install((void*)0x4CB808, (void*)hook_pay_unit_price);
}
