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
};

static double __thiscall hook_hill_bonus(Unit* attacker, Unit* target) {
  float downhill_modifier = 1.25f;
  float uphill_modifier = 0.75f;

  auto player = attacker->owner();
  auto downhill_bonus = player->attribute(DOWNHILL_BONUS_ATTRIBUTE, 0.0f);
  auto uphill_bonus = player->attribute(UPHILL_BONUS_ATTRIBUTE, 0.0f);

  printf("hill bonus down = %f up = %f\n", downhill_bonus, uphill_bonus);

  auto original = (double __thiscall (*)(Unit*, Unit*))0x4C2A70;

  if (downhill_bonus == 0.0f && uphill_modifier == 0.0f) {
    return original(attacker, target);
  }

  downhill_modifier += downhill_bonus;
  uphill_modifier -= uphill_bonus;

  BytesHook downhill_hook((void*)0x4C2A92,
                          reinterpret_cast<char*>(&downhill_modifier),
                          sizeof(downhill_modifier));
  BytesHook uphill_hook((void*)0x4C2AA9,
                        reinterpret_cast<char*>(&uphill_modifier),
                        sizeof(uphill_modifier));

  return original(attacker, target);
}

static VtblHook unit_hook_;
static CallHook building_hook_;
void HillBonus::install() {
  unit_hook_.install((void*)0x63BC0C, (void*)hook_hill_bonus);
  building_hook_.install((void*)0x4CC0A9, (void*)hook_hill_bonus);
}
