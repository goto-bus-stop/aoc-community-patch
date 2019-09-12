#include "queueable_tech.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/attributes.h"
#include "../game/player.h"
#include "../game/unit.h"
#include <cmath> // NAN
#include <cstdint>

/// Bypass pop cap by pretending that queueable tech units have an invalid pop
/// value.
static double THISCALL(get_unit_pop_count, Player* player, int16_t type_id) {
  auto original = getMethod<double, Player*, int16_t>(0x45E360);
  auto unit_type = player->unitType(type_id);
  if (unit_type->isQueueableTech()) {
    return NAN;
  }

  return original(player, type_id);
}

struct TrainableUnit {
public:
  char* name;
  int16_t id;
  int16_t pict;
  int16_t inventory_item_1;
  float inventory_cost_1;
  int16_t inventory_item_2;
  float inventory_cost_2;
  int16_t inventory_item_3;
  float inventory_cost_3;
  int32_t help_string_id;
  int32_t help_page_id;
};

/// Don't show the gather point action.
static int16_t THISCALL(configure_button, void* screen, void* button_shapes,
                        int16_t button_index, int16_t pic_index, int32_t id,
                        int32_t id2, int32_t help_string_id,
                        int32_t help_page_id, int32_t hotkey, char* color_table,
                        char* text2, char* text_msg, int32_t make_disabled) {
  auto selected_unit = *reinterpret_cast<Unit**>((size_t)screen + 0x1230);
  // Action `id == 51` is the Set Gather Point action.
  if (id == 51 && selected_unit != nullptr) {
    auto player = Game::getInstance()->player();
    // `screen` contains a structure with minimal data about the units that can
    // be trained by the selected building. this is used to determine whether to
    // draw the Gather Point action; if we arrived here, the game has already
    // determined that the gather point is required. If all the trainable units
    // are queueable techs, we bail out, and do not draw the gather point
    // action.
    auto num_trainable_units =
        *reinterpret_cast<int16_t*>((size_t)screen + 0x124A);
    auto trainable_units =
        *reinterpret_cast<TrainableUnit**>((size_t)screen + 0x123C);
    bool has_real_unit = false;
    for (auto i = 0; i < num_trainable_units; i += 1) {
      auto unit = player->unitType(trainable_units[i].id);
      if (unit == nullptr || unit->isQueueableTech()) {
        has_real_unit = true;
      }
    }

    if (!has_real_unit) {
      return button_index;
    }
  }

  auto original =
      getMethod<int16_t, void*, void*, int16_t, int16_t, int32_t, int32_t,
                int32_t, int32_t, int32_t, char*, char*, char*, int32_t>(
          0x520620);
  return original(screen, button_shapes, button_index, pic_index, id, id2,
                  help_string_id, help_page_id, hotkey, color_table, text2,
                  text_msg, make_disabled);
}

/// Use "Researching" string for queueable techs, instead of "Creating".
static bool __stdcall get_string(int32_t lang_id, char* output,
                                 size_t output_size) {
  if (lang_id == 20605) { // "Creating"
    // `output` is a stack variable. The unit type is also on the stack when
    // this function is called.
    auto unit_type = *reinterpret_cast<UnitType**>((size_t)output - 0x90);
    if (unit_type != nullptr && unit_type->isQueueableTech()) {
      lang_id = 20600; // "Researching"
    }
  }
  Game::getInstance()->getString(lang_id, output, output_size);
  return true;
}

static CallHook pop_cap_hook_;
static CallHook button_hook_;
static CallHook lang_hook_;
void QueueableTech::install() {
  pop_cap_hook_.install((void*)0x45E360, (void*)get_unit_pop_count);
  button_hook_.install((void*)0x528483, (void*)configure_button);
  lang_hook_.install((void*)0x44FEF0, (void*)get_string);
}
