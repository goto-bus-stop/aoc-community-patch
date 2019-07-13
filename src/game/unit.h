#pragma once
#include "../call_conventions.h"
#include <cstdint>

// Do #include "player.h" to actually use its methods.
class Player;

class Unit {
public:
  inline Player* owner() const {
    return *reinterpret_cast<Player**>((size_t)this + 0xC);
  }
  inline bool isHigherThan(Unit* other_unit) const {
    auto original = THISCALL_PTR(bool, const Unit*, Unit*) 0x4C4990;
    return original(this, other_unit);
  }
  inline float zPos() const {
    return *reinterpret_cast<float*>((size_t)this + 0x40);
  }
  inline bool isCharging() const {
    auto original = THISCALL_PTR(bool, const Unit*) 0x4C5F10;
    return original(this);
  }
};
