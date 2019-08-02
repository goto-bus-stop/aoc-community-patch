#pragma once
#include "attributes.h"
#include <cmath>
#include <cstdint>

class Player {
public:
  inline float attribute(Attribute type, float default_value = NAN) const {
    auto num_attributes = *reinterpret_cast<int32_t*>((size_t)this + 0xA4);
    auto attributes = *reinterpret_cast<float**>((size_t)this + 0xA8);

    auto index = static_cast<int32_t>(type);
    if (index < 0 || index >= num_attributes)
      return default_value;
    return attributes[index];
  }
  inline void addAttribute(Attribute attr, float value, bool earned = false) {
    auto original = getMethod<void, Player*, int16_t, float, bool>(0x45A990);
    original(this, static_cast<int16_t>(attr), value, earned);
  }
};
