#pragma once
#include "../auto_hook.h"
#include "attributes.h"
#include "game.h"
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

// The bits in the Hide In Editor value are, low to high:
// - Hidden in scenario editor
// - Queueable tech
struct HideEditorFlags {
public:
  uint8_t value;

  inline bool hideInEditor() const { return (value & 1) == 1; }
  inline bool queueableTech() const { return (value & 2) == 2; }
};

enum class UnitClass : int16_t {
  AllClasses = -1,
  Archer = 0,
  Artifact = 1,
  TradeBoat = 2,
  Building = 3,
  Civilian = 4,
  OceanFish = 5,
  Infantry = 6,
  BerryBush = 7,
  StoneMine = 8,
  PreyAnimal = 9,
  PredatorAnimal = 10,
  Miscellaneous = 11,
  Cavalry = 12,
  SiegeWeapon = 0xD,
  Terrain = 0xE,
  Tree = 0xF,
  TreeStump = 0x10,
  Healer = 0x11,
  Monk = 0x12,
  TradeCart = 0x13,
  TransportBoat = 0x14,
  FishingBoat = 0x15,
  Warship = 0x16,
  Conquistador = 0x17,
  WarElephant = 0x18,
  Hero = 0x19,
  ElephantArcher = 0x1A,
  Wall = 0x1B,
  Phalanx = 0x1C,
  DomesticAnimal = 0x1D,
  Flag = 0x1E,
  DeepSeaFish = 0x1F,
  GoldMine = 0x20,
  ShoreFish = 0x21,
  Cliff = 0x22,
  Petard = 0x23,
  CavalryArcer = 0x24,
  Doppelganger = 0x25,
  Bird = 0x26,
  Gate = 0x27,
  SalvagePile = 0x28,
  ResourcePile = 0x29,
  Relic = 0x2A,
  MonkWithRelic = 0x2B,
  HandCannoneer = 0x2C,
  TwoHandedSwordsman = 0x2D,
  Pikeman = 0x2E,
  Scout = 0x2F,
  OreMine = 0x30,
  Farm = 0x31,
  Spearman = 0x32,
  PackedUnit = 0x33,
  Tower = 0x34,
  BoardingBoat = 0x35,
  UnpackedSiegeUnit = 0x36,
  Ballista = 0x37,
  Raider = 0x38,
  CavalryRaider = 0x39,
  Livestock = 0x3A,
  King = 0x3B,
  MiscBuilding = 0x3C,
  ControlledAnimal = 0x3D,
};

class UnitType {
public:
  /// Get the unit class of this unit type.
  inline UnitClass unitClass() const {
    return *reinterpret_cast<UnitClass*>((size_t)this + 0x16);
  }

  /// Get the unique ID of this unit type.
  inline int16_t id() const {
    return *reinterpret_cast<int16_t*>((size_t)this + 0x10);
  }

  /// Get the string ID of the name of this unit type.
  inline int16_t nameStringId() const {
    return *reinterpret_cast<int16_t*>((size_t)this + 0x0C);
  }

  /// Get the name of this unit type.
  inline const char* name() const {
    return Game::getInstance()->getString(this->nameStringId());
  }

  /// Get the attributes of this unit type. These can be attributes contained in
  /// the unit type, like with resource stockpiles, or attribute "effects" that
  /// give or take resources from the player.
  UnitAttributes attributes() const {
    return UnitAttributes(reinterpret_cast<AttributeMode*>((size_t)this + 0x90),
                          reinterpret_cast<Attribute*>((size_t)this + 0x72),
                          reinterpret_cast<float*>((size_t)this + 0x78));
  }

  inline HideEditorFlags newFlags() const {
    return HideEditorFlags{
      *reinterpret_cast<uint8_t*>((size_t)this + 0x56)
    };
  }

  /// Check if this unit has the "queueable tech" flag set.
  inline bool isQueueableTech() const {
    return this->newFlags().queueableTech();
  }
};

class Unit {
public:
  /// Get the ID of this unit.
  inline int32_t id() const {
    return *reinterpret_cast<int32_t*>((size_t)this + 0x4);
  }
  /// Get the base type ID of this unit, indicating whether it's a
  /// static, action, moving, combat unit etc.
  inline uint8_t baseTypeId() const {
    return *reinterpret_cast<uint8_t*>((size_t)this + 0x4E);
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

  /// Check if the unit is idle.
  inline bool isIdle() const {
    auto original = getMethod<bool, const Unit*>(0x5FF6F0);
    return this->baseTypeId() >= 40 && original(this);
  }
};
