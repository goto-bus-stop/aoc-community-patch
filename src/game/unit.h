#pragma once
#include "../auto_hook.h"
#include "attributes.h"
#include <cstdint>

// Do #include "player.h" to actually use its methods.
class Player;

class UnitAttributes {
private:
  AttributeMode* mode_;
  Attribute* type_;
  float* amount_;

public:
  UnitAttributes(AttributeMode* modes, Attribute* types, float* amounts)
      : mode_(modes), type_(types), amount_(amounts) {}

  /// Get the attribute mode for the given index.
  inline AttributeMode mode(int32_t index) { return this->mode_[index]; }
  /// Get the attribute type for the given index.
  inline Attribute type(int32_t index) { return this->type_[index]; }
  /// Get the attribute value for the given index.
  inline float amount(int32_t index) { return this->amount_[index]; }
};

class UnitType {
public:
  /// Get the unit class of this unit type.
  inline int16_t unitClass() const {
    return *reinterpret_cast<int16_t*>((size_t)this + 0x16);
  }

  /// Get the internal name of this unit type.
  inline const char* name() const {
    auto lang_id = *reinterpret_cast<int16_t*>((size_t)this + 0xC);
    auto get_string = getMethod<const char*, void*, int16_t>(0x43A700);
    return get_string(*(void**)0x7912A0, lang_id);
  }

  /// Get the attributes of this unit type. These can be attributes contained in
  /// the unit type, like with resource stockpiles, or attribute "effects" that
  /// give or take resources from the player.
  UnitAttributes attributes() const {
    return UnitAttributes(reinterpret_cast<AttributeMode*>((size_t)this + 0x90),
                          reinterpret_cast<Attribute*>((size_t) + 0x72),
                          reinterpret_cast<float*>((size_t) + 0x78));
  }
};

class Unit {
public:
  /// Get the ID of this unit.
  inline int32_t id() const {
    return *reinterpret_cast<int32_t*>((size_t)this + 0x4);
  }
  /// Get the unit type.
  inline UnitType* type() const {
    return *reinterpret_cast<UnitType**>((size_t)this + 0x8);
  }
  /// Get the player who controls this unit.
  inline Player* owner() const {
    return *reinterpret_cast<Player**>((size_t)this + 0xC);
  }
  /// Check if the unit is on higher elevation than some other unit.
  inline bool isHigherThan(Unit* other_unit) const {
    auto original = getMethod<bool, const Unit*, Unit*>(0x4C4990);
    return original(this, other_unit);
  }
  /// Get the elevation level this unit is on.
  inline float zPos() const {
    return *reinterpret_cast<float*>((size_t)this + 0x40);
  }
  /// Check if the unit is "charging" (obsolete ability).
  inline bool isCharging() const {
    auto original = getMethod<bool, const Unit*>(0x4C5F10);
    return original(this);
  }
};
