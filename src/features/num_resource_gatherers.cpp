#include "num_resource_gatherers.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/game.h"
#include "../game/player.h"
#include "../game/unit.h"
#include "../game/draw_area.h"
#include <array>
#include <cstdint>
#include <wingdi.h>

template <const Attribute Resource>
static int32_t count_gatherers() {
  auto player = Game::getInstance()->player();
  auto units = player->units();

  int32_t count = 0;
  for (const auto unit : units) {
    if (unit->type()->unitClass() == UnitClass::FishingBoat) {
      if constexpr (Resource == Attribute::FoodStorage) {
        if (!unit->isIdle()) {
          count += 1;
        }
      }
      continue;
    }

    if (unit->type()->unitClass() != UnitClass::Civilian) {
      continue;
    }

    if (unit->isIdle()) {
      continue;
    }

    switch (unit->type()->id()) {
    case 579: // Gold Miner
    case 581: // Gold Miner
      if constexpr (Resource == Attribute::GoldStorage) {
        count += 1;
      }
      break;
    case 56: // Fisher
    case 57: // Fisher
    case 120: // Forager
    case 122: // Hunter
    case 214: // Farmer
    case 216: // Hunter
    case 259: // Farmer
    case 354: // Forager
    case 590: // Shepherd
    case 592: // Shepherd
      if constexpr (Resource == Attribute::FoodStorage) {
        count += 1;
      }
      break;
    case 123: // Lumberjack
    case 218: // Lumberjack
      if constexpr (Resource == Attribute::WoodStorage) {
        count += 1;
      }
      break;
    case 124: // Stone Miner
    case 220: // Stone Miner
      if constexpr (Resource == Attribute::StoneStorage) {
        count += 1;
      }
      break;
    }
  }

  return count;
}

/// Adds villager count labelling to `TRIBE_Panel_Inven`.
static void THISCALL(draw_item, void* inventory, int32_t index, int32_t x, int32_t y, int32_t num_arguments, int32_t arg1, int32_t arg2) {
  if (num_arguments == 1) {
    switch (index) {
    case 0:
      arg2 = count_gatherers<Attribute::FoodStorage>();
      break;
    case 1:
      arg2 = count_gatherers<Attribute::WoodStorage>();
      break;
    case 2:
      arg2 = count_gatherers<Attribute::GoldStorage>();
      break;
    case 3:
      arg2 = count_gatherers<Attribute::StoneStorage>();
      break;
    }

    // just use the classic style
    if (arg2 == 0) {
      auto original = getMethod<void, void*, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t>(0x452A80);
      original(inventory, index, x, y, num_arguments, arg1, arg2);
      return;
    }

    num_arguments = 2;
  }

  auto x_offset = *reinterpret_cast<int32_t*>((size_t)inventory + 0xC);
  auto y_offset = *reinterpret_cast<int32_t*>((size_t)inventory + 0x10);
  auto draw_area = *reinterpret_cast<DrawArea**>((size_t)inventory + 0x20);
  auto font_color = *reinterpret_cast<uint32_t*>((size_t)inventory + 0x128);
  auto background_color = *reinterpret_cast<uint32_t*>((size_t)inventory + 0x12C);

  x += x_offset;
  y += y_offset;

  char message[40];
  switch (num_arguments) {
  case 2:
    sprintf(message, "%d / %d", arg1, arg2);
    break;
  case 1:
    sprintf(message, "%d", arg1);
    break;
  default:
    strcpy(message, " ");
    break;
  }
  auto length = strlen(message);

  SetTextColor(draw_area->deviceContext(), background_color);
  TextOut(draw_area->deviceContext(), x - 1, y + 1, message, length);
  SetTextColor(draw_area->deviceContext(), font_color);
  TextOut(draw_area->deviceContext(), x, y, message, length);
}

static std::array<CallHook, 4> draw_hook_;
void NumResourceGatherers::install() {
  draw_hook_[0].install((void*)0x452863, (void*)draw_item);
  draw_hook_[1].install((void*)0x45288A, (void*)draw_item);
  draw_hook_[2].install((void*)0x4528B3, (void*)draw_item);
  draw_hook_[3].install((void*)0x4528DC, (void*)draw_item);
}
