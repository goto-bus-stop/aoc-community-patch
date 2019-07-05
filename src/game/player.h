#pragma once
#include <cmath>
#include <cstdint>

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
