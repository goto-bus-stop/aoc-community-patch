#include "num_idles.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/game.h"
#include "../game/player.h"
#include "../game/unit.h"
#include "../game/draw_area.h"

static size_t get_num_idles() {
  auto player = Game::getInstance()->player();
  auto units = player->units();

  size_t count = 0;
  for (const auto unit : units) {
    auto unit_class = unit->type()->unitClass();
    auto is_civilian = unit_class == UnitClass::Civilian
      || unit_class == UnitClass::FishingBoat
      || unit_class == UnitClass::TradeCart
      || unit_class == UnitClass::TradeBoat;
    if (is_civilian) {
      count += unit->isIdle();
    }
  }
  return count;
}

static bool is_idles_button(void* button) {
  return button != nullptr && *reinterpret_cast<int32_t*>((size_t)button + 0xF8) == 161;
}

static void* get_idles_button(void* screen) {
  auto button = *reinterpret_cast<void**>((size_t)screen + 0x1080 + 25 * 4);
  if (is_idles_button(button)) {
    return button;
  }
  return nullptr;
}

static void update_idles_button(void* screen) {
  auto button = get_idles_button(screen);
  if (button == nullptr) {
    return;
  }

  auto set_number_display_type = getMethod<void, void*, int32_t>(0x453B70);
  auto set_number_display_value = getMethod<void, void*, int32_t, int32_t>(0x453B90);
  auto count = get_num_idles();
  set_number_display_type(button, 2); // only display if nonzero
  set_number_display_value(button, count, 0);
}

/// Adds "garrison" labelling to `TRIBE_Panel_Button`s, like the idle villager button.
static void THISCALL(fancier_draw, void* button) {
  auto original = getMethod<void, void*>(0x551940);
  original(button);

  auto garrison_display_type = *reinterpret_cast<int32_t*>((size_t)button + 0x2D8);
  auto garrison_number = *reinterpret_cast<int32_t*>((size_t)button + 0x2DC);
  auto x_offset = *reinterpret_cast<int32_t*>((size_t)button + 0xC);
  auto y_offset = *reinterpret_cast<int32_t*>((size_t)button + 0x10);
  auto draw_area = *reinterpret_cast<DrawArea**>((size_t)button + 0x20);
  auto clip_region = *reinterpret_cast<HRGN*>((size_t)button + 0x8C);
  auto font = *reinterpret_cast<HFONT*>((size_t)button + 0x1F0);

  if (garrison_display_type && (garrison_number > 0 || garrison_display_type != 2)) {
    if (auto context = draw_area->getDeviceContext("tpnl_btn::draw2")) {
      SelectClipRgn(context, clip_region);
      auto old_font = SelectObject(context, font);
      SetBkMode(context, TRANSPARENT);
      char label[10];
      sprintf(label, "%d", garrison_number);
      auto c = strlen(label);
      SetTextColor(context, 0x000000);
      TextOutA(context, x_offset + 2, y_offset + 1, label, c);
      TextOutA(context, x_offset + 2, y_offset + 3, label, c);
      TextOutA(context, x_offset + 4, y_offset + 1, label, c);
      TextOutA(context, x_offset + 4, y_offset + 3, label, c);
      SetTextColor(context, 0xFFFFFF);
      TextOutA(context, x_offset + 3, y_offset + 2, label, c);
      SelectClipRgn(context, 0);
      SelectObject(context, old_font);
      draw_area->releaseDeviceContext("tpnl_inv::draw2");
    }
  }
}

static int32_t THISCALL(handle_paint, void* screen) {
  update_idles_button(screen);

  auto original = getMethod<int32_t, void*>(0x51B2D0);
  return original(screen);
}

static VtblHook update_hook_;
static CallHook draw_hook_;
void NumIdles::install() {
  update_hook_.install((void*)0x63F36C, (void*)handle_paint);
  draw_hook_.install((void*)0x4534AF, (void*)fancier_draw);
}
