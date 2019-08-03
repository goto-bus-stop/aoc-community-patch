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
    log_message(*(void**)0x7911F8, "Community Patch", "Trigger tech 708", -1, 0, 0);
    player->triggerTech(DisableKipchakTech);
  }
}

void THISCALL(hook_add_attribute, Player* player, int16_t attribute_id,
                                   float value, int32_t flag) {
  auto original = getMethod<void, Player*, int16_t, float, int32_t>(0x45A990);
  original(player, attribute_id, value, flag);

  char prefix[100];
  sprintf(prefix, "Attr %d", attribute_id);
  char message[100];
  sprintf(message, "%f new value=%f", value, player->attribute(static_cast<Attribute>(attribute_id)));

  auto log_message = getMethod<int32_t, void*, const char*, const char*,
                               int32_t, int32_t, int32_t>(0x5E2780);
  if (attribute_id > 200) {
    log_message(*(void**)0x7911F8, prefix, message, -1, 0, 0);
  }

  // When MercenaryKipchakCount reaches 0, trigger DisableKipchakTech
  if (attribute_id == static_cast<int16_t>(Attribute::MercenaryKipchakCount)) {
    check_kipchak(player);
  }
}

int8_t THISCALL(hook_pay_unit_price, size_t player, int16_t unit_id, float modifier, int32_t ignore_pop) {
  printf("hook_pay_unit_price(%x, %d, %f, %d)\n", player, unit_id, modifier, ignore_pop);
  fflush(stdout);
  auto original = getMethod<int32_t, size_t, int16_t, float, int32_t>(0x457E40);
  auto result = original(player, unit_id, modifier, ignore_pop);
  if (result) {
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
