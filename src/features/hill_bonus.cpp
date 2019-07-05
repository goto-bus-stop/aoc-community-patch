#include "hill_bonus.h"
#include "../auto_hook.h"
#include <cmath>
#include <cstdint>
#include <cstdio>

static const auto DOWNHILL_BONUS_ATTRIBUTE = 211u;
static const auto UPHILL_BONUS_ATTRIBUTE = 212u;

class Player {
public:
  inline float attribute(uint32_t index, float default_value = NAN) const {
    auto num_attributes = *reinterpret_cast<uint32_t*>((size_t)this + 0xA4);
    auto attributes = *reinterpret_cast<float**>((size_t)this + 0xA8);

    if (index >= num_attributes)
      return default_value;
    return attributes[index];
  }
};

class Unit {
public:
  inline Player* owner() const {
    return *reinterpret_cast<Player**>((size_t)this + 0xC);
  }
  inline bool isHigherThan(Unit* other_unit) const {
    auto original = (bool __thiscall (*)(const Unit*, Unit*))0x4C4990;
    return original(this, other_unit);
  }
  inline float zPos() const {
    return *reinterpret_cast<float*>((size_t)this + 0x40);
  }
  inline bool isCharging() const {
    auto original = (bool __thiscall (*)(const Unit*))0x4C5F10;
    return original(this);
  }
};

static double __thiscall hook_hill_bonus(Unit* attacker, Unit* target) {
  float downhill_modifier = 1.25f;
  float uphill_modifier = 0.75f;

  auto player = attacker->owner();
  auto downhill_bonus = player->attribute(DOWNHILL_BONUS_ATTRIBUTE, 0.0f);
  auto uphill_bonus = player->attribute(UPHILL_BONUS_ATTRIBUTE, 0.0f);

  auto original = (double __thiscall (*)(Unit*, Unit*))0x4C2A70;

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
