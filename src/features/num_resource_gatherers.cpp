#include "num_resource_gatherers.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/game.h"
#include "../game/player.h"
#include "../game/unit.h"
#include "../game/draw_area.h"
#include "../game/panel.h"
#include "../game/shape.h"
#include <algorithm>
#include <array>
#include <chrono>
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

/*
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
*/

class ResourcePanel : public Panel {
protected:
  Player* player_;
  HFONT font_;
  uint32_t background_color_ = 0x00'00'00;
  uint32_t font_color_ = 0xFF'FF'FF;
  Shape resource_icons_{"resico.shp", 50760};
  Shape flash_icons_{"pocap.shp", 53002};
  std::chrono::system_clock::time_point idle_time_;
  uint32_t idle_interval_ = 500;
  std::array<bool, 7> highlight_;
  std::array<RECT, 7> item_rects_;
  std::chrono::system_clock::time_point last_flash_time_;
  bool any_highlight_ = false;
  bool flashing_ = false;

  float saved_food_ = 0.0;
  float saved_wood_ = 0.0;
  float saved_stone_ = 0.0;
  float saved_gold_ = 0.0;
  float saved_ore_ = 0.0;
  float saved_pop_ = 0.0;
  float saved_max_pop_ = 0.0;

public:
  ResourcePanel(DrawArea* draw_area, Panel* parent, HFONT font, Player* player)
    : player_(player),
      font_(font)
  {
    this->vtable_ = reinterpret_cast<PanelVTable*>(0x637700);

    this->highlight_.fill(false);
    this->item_rects_.fill({0, 0, 0, 0});

    if (!this->vtable_->setup(this, draw_area, parent, 0, 0, 0, 0, 0)) {
      this->error_code_ = 1;
    }

    setInstance(this);
  }

  ~ResourcePanel() {
    setInstance(nullptr);
  }


  void setTextColor(uint32_t font_color, uint32_t back_color) {
    this->font_color_ = font_color;
    this->background_color_ = back_color;
    this->setRedraw();
  }

  void setPlayer(Player* player) {
    this->player_ = player;
    this->saveInfo();
    this->setRedraw();
  }

  void setHighlight(uint8_t attribute_id, bool highlight) {
    this->highlight_[attribute_id] = highlight;
    this->any_highlight_ = std::any_of(this->highlight_.begin(), this->highlight_.end(), [](auto h) { return h; });
    this->setRedraw();
  }

  void saveInfo() {
    if (this->player_ == nullptr)
      return;

    this->saved_food_ = this->player_->attribute(Attribute::FoodStorage);
    this->saved_wood_ = this->player_->attribute(Attribute::WoodStorage);
    this->saved_stone_ = this->player_->attribute(Attribute::StoneStorage);
    this->saved_gold_ = this->player_->attribute(Attribute::GoldStorage);
    this->saved_ore_ = this->player_->attribute(Attribute::OreStorage);
    this->saved_pop_ = this->player_->attribute(Attribute::CurrentPopulation);
    this->saved_max_pop_ = this->player_->attribute(Attribute::PopulationHeadroom);
  }

  void drawItem(int32_t item, int32_t x, int32_t y, int8_t num_count, int32_t num1, int32_t num2) {
    printf("[aoc-community-patch] ResourcePanel::drawItem(%d, x=%d, y=%d, count=%d, 1=%d, 2=%d)\n", item, x, y, num_count, num1, num2);
    x += this->x_offset_;
    y += this->y_offset_;

    char text[100];
    if (num_count == 1) {
      sprintf(text, "%d", num1);
    } else if (num_count == 2) {
      sprintf(text, "%d/%d", num1, num2);
    } else {
      strcpy(text, " ");
    }

    auto dc = this->draw_area_->deviceContext();
    SetTextColor(dc, this->background_color_);
    TextOut(dc, x - 1, y + 1, text, strlen(text));
    SetTextColor(dc, this->font_color_);
    TextOut(dc, x, y, text, strlen(text));
    this->item_rects_[item] = {
      .left = x - 67,
      .top = y,
      .right = x + 2,
      .bottom = y + 15
    };
  }

  void drawItem(int32_t item, int32_t x, int32_t y, int32_t num1) {
    this->drawItem(item, x, y, 1, num1);
  }
  void drawItem(int32_t item, int32_t x, int32_t y, int32_t num1, int32_t num2) {
    this->drawItem(item, x, y, 2, num1, num2);
  }

  int32_t getMessage(int32_t x, int32_t y, bool extended) const {
    int32_t hovering_index = 0;
    for (auto& rect : this->item_rects_) {
      hovering_index += 1;
      if (x >= rect.left && x <= rect.right && y >= rect.top && y >= rect.bottom) {
        break;
      }
    }

    return extended
      ? 43200 + hovering_index
      : 42200 + hovering_index;
  }

  void draw() {
    this->redraw_mode_ = RedrawMode::None;

    if (this->parent_ != nullptr)
      this->parent_->drawRect(this->rect_);

    if (this->player_ == nullptr)
      return;

    this->drawSetup();
    if (auto dc = this->draw_area_->getDeviceContext("ResourcePanel::draw")) {
      SelectClipRgn(dc, this->clip_region_);
      auto prev_hfont = SelectObject(dc, this->font_);
      auto prev_align = SetTextAlign(dc, 2);
      SetBkMode(dc, 1);

      auto p = this->player_;
      this->drawItem(1, 75, 4, p->attribute(Attribute::WoodStorage));
      this->drawItem(0, 152, 4, p->attribute(Attribute::FoodStorage));
      this->drawItem(2, 229, 4, p->attribute(Attribute::GoldStorage));
      this->drawItem(3, 306, 4, p->attribute(Attribute::StoneStorage));
      auto max_pop = std::max(
          p->attribute(Attribute::BonusPopulationCap),
          p->attribute(Attribute::CurrentPopulation) + p->attribute(Attribute::PopulationHeadroom)
      );
      this->drawItem(4, 383, 4, p->attribute(Attribute::CurrentPopulation), max_pop);

      SelectObject(dc, prev_hfont);
      SetTextAlign(dc, prev_align);
      SelectClipRgn(dc, nullptr);
      this->draw_area_->releaseDeviceContext("ResourcePanel::draw");
    }

    this->drawFinish();
  }

  void handleIdle() {
    Panel::handleIdle();

    if (this->player_ == nullptr)
      return;

    auto now = std::chrono::system_clock::now();
    constexpr auto flash_interval = std::chrono::milliseconds(750);
    if (this->any_highlight_ && now - this->last_flash_time_ >= flash_interval) {
      this->flashing_ = !this->flashing_;
      this->last_flash_time_ = now;
      this->setRedraw();
    }

    constexpr auto idle_interval = std::chrono::milliseconds(500);
    if (now - this->idle_time_ >= idle_interval) {
      this->idle_time_ = now;
      if (this->saved_food_ == this->player_->attribute(Attribute::FoodStorage) &&
          this->saved_wood_ == this->player_->attribute(Attribute::WoodStorage) &&
          this->saved_stone_ == this->player_->attribute(Attribute::StoneStorage) &&
          this->saved_gold_ == this->player_->attribute(Attribute::GoldStorage) &&
          this->saved_ore_ == this->player_->attribute(Attribute::OreStorage) &&
          this->saved_pop_ == this->player_->attribute(Attribute::CurrentPopulation) &&
          this->saved_max_pop_ == this->player_->attribute(Attribute::PopulationHeadroom)) {
        return;
      }

      this->saveInfo();
      this->setRedraw();
    }
  }

  bool getHelpInfo(char*& help_text, int32_t& help_page, int32_t x, int32_t y) {
    if (!this->is_active_)
      return false;
    if (!this->isInside(x, y))
      return false;

    // TODO actually get the help info

    x -= this->x_offset_;
    if (x >= 165) {
      help_page = -1;
      return true;
    }
    if (x >= 110) {
      help_page = -1;
      return true;
    }
    if (x >= 55) {
      help_page = -1;
      return true;
    }
    if (x >= 0) {
      help_page = -1;
      return true;
    }

    return false;
  }

  bool pointingAt(int32_t x, int32_t y, int32_t& item, int32_t& sub_item, int32_t& extended, int32_t& string_id, char* text, int32_t max_text) {
    if (!this->isInside(x, y))
      return false;

    extended = true; // Game::getInstance()->rollover
    string_id = this->getMessage(x, y, extended);
    item = string_id != -1 ? string_id : 0;
    sub_item = 0;
    if (text != nullptr)
      text[0] = '\0';
    return true;
  }

  static void setInstance(ResourcePanel* panel) {
    *reinterpret_cast<ResourcePanel**>(0x695794) = panel;
  }

  static size_t memory_size_;
};

size_t ResourcePanel::memory_size_ = sizeof(ResourcePanel);

static void THISCALL(set_text_color, ResourcePanel* panel, uint32_t font_color, uint32_t background_color) {
  panel->setTextColor(font_color, background_color);
}

static void THISCALL(set_player, ResourcePanel* panel, Player* player) {
  panel->setPlayer(player);
}

static void THISCALL(set_highlight, ResourcePanel* panel, uint8_t attr, bool enabled) {
  panel->setHighlight(attr, enabled);
}

static void THISCALL(draw, ResourcePanel* panel) {
  panel->draw();
}

static bool THISCALL(get_help_info, ResourcePanel* panel, char** help_text, int32_t* help_page, int32_t x, int32_t y) {
  return panel->getHelpInfo(*help_text, *help_page, x, y);
}

static bool THISCALL(pointing_at, ResourcePanel* panel, int32_t x, int32_t y, int32_t* item, int32_t* sub_item, int32_t* extended, int32_t* string_id, char* text, int32_t max_text) {
  return panel->pointingAt(x, y, *item, *sub_item, *extended, *string_id, text, max_text);
}

static void THISCALL(handle_idle, ResourcePanel* panel) {
  panel->handleIdle();
}

static ResourcePanel* THISCALL(constructor, uint8_t* buffer, DrawArea* draw_area, Panel* parent, HFONT font, Player* player) {
  return new(buffer) ResourcePanel(draw_area, parent, font, player);
}

static void THISCALL(destructor, ResourcePanel* panel) {
  delete panel;
}

static BytesHook size_hook_;
static std::array<CallHook, 6> call_hooks_;
static std::array<VtblHook, 5> vtbl_hooks_;
void NumResourceGatherers::install() {
  size_hook_.install((void*)0x517486, reinterpret_cast<const void*>(&ResourcePanel::memory_size_), 4);
  call_hooks_[0].install((void*)0x51FCE2, (void*)set_text_color);
  call_hooks_[1].install((void*)0x51F956, (void*)set_player);
  call_hooks_[2].install((void*)0x43CDFD, (void*)set_highlight);
  call_hooks_[3].install((void*)0x441C5B, (void*)set_highlight);
  call_hooks_[4].install((void*)0x441C99, (void*)set_highlight);
  call_hooks_[5].install((void*)0x5174BD, (void*)constructor);
  vtbl_hooks_[0].install((void*)0x637730, (void*)draw);
  vtbl_hooks_[1].install((void*)0x63774C, (void*)handle_idle);
  vtbl_hooks_[2].install((void*)0x6377E0, (void*)get_help_info);
  vtbl_hooks_[3].install((void*)0x6377F4, (void*)pointing_at);
  vtbl_hooks_[4].install((void*)0x637700, (void*)destructor);
}
