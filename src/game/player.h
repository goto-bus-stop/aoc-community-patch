#pragma once
#include "attributes.h"
#include <cmath>
#include <cstdint>

class Player {
public:
  /// Get a player attribute.
  ///
  /// Returns default value if the player does not have the given attribute.
  inline float attribute(Attribute type, float default_value = NAN) const {
    auto num_attributes = *reinterpret_cast<int32_t*>((size_t)this + 0xA4);
    auto attributes = *reinterpret_cast<float**>((size_t)this + 0xA8);

    auto index = static_cast<int32_t>(type);
    if (index < 0 || index >= num_attributes)
      return default_value;
    return attributes[index];
  }

  /// Add (+) a value to the given player attribute.
  ///
  /// Set earned = true to make this addition count towards total resource collection achievements.
  inline void addAttribute(Attribute attr, float value, bool earned = false) {
    auto original = getMethod<void, Player*, int16_t, float, bool>(0x45A990);
    original(this, static_cast<int16_t>(attr), value, earned);
  }
};
