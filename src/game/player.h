#pragma once
#include "../auto_hook.h"
#include "attributes.h"
#include <cmath>
#include <cstdint>

/// Include "unit.h" to use this class's methods.
class UnitType;

class Player {
public:
  /// Get the number of attributes this player has.
  inline int32_t numAttributes() const {
    return *reinterpret_cast<int32_t*>((size_t)this + 0xA4);
  }

  /// Get the list of attributes this player has.
  inline float* attributes() const {
    return *reinterpret_cast<float**>((size_t)this + 0xA8);
  }

  /// Get a player attribute.
  ///
  /// Returns default value if the player does not have the given attribute.
  inline float attribute(Attribute attr, float default_value = NAN) const {
    auto index = static_cast<int32_t>(attr);
    if (index < 0 || index >= this->numAttributes())
      return default_value;

    auto attributes = this->attributes();
    return attributes[index];
  }

  /// Add (+) a value to the given player attribute.
  ///
  /// Set earned = true to make this addition count towards total resource
  /// collection achievements.
  inline void addAttribute(Attribute attr, float value, bool earned = false) {
    auto original = getMethod<void, Player*, int16_t, float, bool>(0x45A990);
    original(this, static_cast<int16_t>(attr), value, earned);
  }

  /// Trigger a tech.
  inline void triggerTech(int16_t tech_id) {
    auto techs = *reinterpret_cast<void**>((size_t)this + 0x12A0);
    auto original = getMethod<void, void*, int16_t>(0x40244D);
    original(techs, tech_id);
  }

  /// Get a unit type that can be created by this player.
  inline UnitType* unitType(int16_t type_id) const {
    if (type_id < 0) return nullptr;
    auto num_unit_types = *reinterpret_cast<int32_t*>((size_t)this + 0x70);
    if (type_id >= num_unit_types) return nullptr;

    auto unit_types = *reinterpret_cast<UnitType***>((size_t)this + 0x74);
    return unit_types[type_id];
  }
};
