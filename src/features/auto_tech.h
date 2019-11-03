#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include "../game/player.h"

class TechIdArray {
  size_t size_ = 0;
  int16_t* elements_  = nullptr;

public:
  constexpr TechIdArray(size_t size) : size_(size) {
    if (size > 0) elements_ = new int16_t[size];
  }

  constexpr size_t size() const { return size_; }
  constexpr int16_t& operator[](size_t index) { return elements_[index]; }

  constexpr int16_t* begin() const { return elements_; }
  constexpr int16_t* end() const { return elements_ + size_; }
};

class AutoTech {
private:
  using PlayerAttributeTechs = std::unique_ptr<TechIdArray[]>;
  // may_trigger_tech_[player][attribute_id] → list of tech IDs that may be triggered by an update to that attribute
  std::array<PlayerAttributeTechs, 9> may_trigger_tech_;
  
public:
  void onAttributeUpdate(Player* player, Attribute attribute_id);

  static void install();
};
