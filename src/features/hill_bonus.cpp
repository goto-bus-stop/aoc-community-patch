#include "hill_bonus.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/player.h"
#include "../game/unit.h"
#include <cstdint>

static const auto DOWNHILL_BONUS_ATTRIBUTE = 211u;
static const auto UPHILL_BONUS_ATTRIBUTE = 212u;

static double THISCALL(hook_hill_bonus, Unit* attacker, Unit* target) {
  float downhill_modifier = 1.25f;
  float uphill_modifier = 0.75f;

  auto player = attacker->owner();
  auto downhill_bonus = player->attribute(DOWNHILL_BONUS_ATTRIBUTE, 0.0f);
  auto uphill_bonus = player->attribute(UPHILL_BONUS_ATTRIBUTE, 0.0f);

  auto original = getMethod<double, Unit*, Unit*>(0x4C2A70);

  if (downhill_bonus == 0.0f && uphill_modifier == 0.0f) {
    return original(attacker, target);
  }

  auto modifier = 1.0f;
  if (attacker->isHigherThan(target)) {
    modifier = downhill_modifier + downhill_bonus;
  } else if (attacker->zPos() < target->zPos()) {
    modifier = uphill_modifier + uphill_bonus;
  }

  if (attacker->isCharging()) {
    modifier *= 2;
  }

  return modifier;
}

static VtblHook unit_hook_;
static CallHook building_hook_;
void HillBonus::install() {
  unit_hook_.install((void*)0x63BC0C, (void*)hook_hill_bonus);
  building_hook_.install((void*)0x4CC0A9, (void*)hook_hill_bonus);
}
