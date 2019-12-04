#include "queueable_tech.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include "../game/attributes.h"
#include "../game/player.h"
#include "../game/unit.h"
#include <array>
#include <cstdint>
#include <cstdio>

static CallHook pop_cap_hook_;
static CallHook button_hook_;
static std::array<CallHook, 2> lang_hook_;
static std::array<CallHook, 2> available_hook_;
static std::array<BytesHook, 4> scen_editor_hook_;

/// Bypass pop cap by pretending that queueable tech units have an invalid pop
/// value.
///
/// This function is called in _one_ place, while the unit is entering the first
/// spot in a building's production queue. At that point, the population is checked in two ways:
///
/// if (CurrentPop + TrainingUnits + NewUnitPopCount > PopCap)
///   disallow;
/// if (Headroom - TrainingUnits - NewUnitPopCount < 0)
///   disallow;
/// start_producing_unit;
///
/// By returning a very large negative number, we can make sure both conditions fail,
/// and the unit production is allowed.
static double THISCALL(get_unit_pop_count, Player* player, int16_t type_id) {
  auto original = getMethod<double, Player*, int16_t>(0x45E360);
  auto unit_type = player->unitType(type_id);
  if (unit_type != nullptr && unit_type->isQueueableTech()) {
    return -1e20;
  }

  return original(player, type_id);
}

struct TrainableUnit {
public:
  char* name = nullptr;
  int16_t id = 0;
  int16_t pict = 0;
  int16_t inventory_item_1 = 0;
  float inventory_cost_1 = 0.0f;
  int16_t inventory_item_2 = 0;
  float inventory_cost_2 = 0.0f;
  int16_t inventory_item_3 = 0;
  float inventory_cost_3 = 0.0f;
  int32_t help_string_id = 0;
  int32_t help_page_id = 0;

  TrainableUnit(const TrainableUnit&) = default;
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
      if (unit == nullptr || !unit->isQueueableTech()) {
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

/// Remove the buttons for queueable techs when they are in-progress.
static void THISCALL(get_available_units, Player* player, TrainableUnit** trainable_units, int16_t* num_trainable_units, int16_t buildable_from) {
  auto original = getMethod<void, Player*, TrainableUnit**, int16_t*, int16_t>(0x456D50);
  original(player, trainable_units, num_trainable_units, buildable_from);

  auto list = *trainable_units;
  for (int16_t i = 0; i < *num_trainable_units; i += 1) {
    auto id = list[i].id;
    auto unit = player->unitType(id);
    if (unit != nullptr && unit->isQueueableTech() && player->unitTypeCount(id) > 0) {
      // Pop the last one and move it into the current slot, overriding the in-progress queueable tech
      *num_trainable_units -= 1;
      list[i] = list[*num_trainable_units];
      i -= 1;
    }
  }
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

static const char* THISCALL(get_string_2, void*, int32_t lang_id) {
  /* std::byte stack_ptr[1]; */
  if (lang_id == 0x29127) {
    // TODO handle the "--%s created--" message
  }
  return Game::getInstance()->getString(lang_id);
}

void QueueableTech::install() {
  pop_cap_hook_.install((void*)0x4CBE8A, (void*)get_unit_pop_count);
  button_hook_.install((void*)0x528483, (void*)configure_button);
  available_hook_[0].install((void*)0x527D37, (void*)get_available_units);
  available_hook_[1].install((void*)0x528449, (void*)get_available_units);
  lang_hook_[0].install((void*)0x44FEF0, (void*)get_string);
  lang_hook_[1].install((void*)0x4401CD, (void*)get_string_2);

  // Change explicit `if (HideInEditor == 1)` check to `if (HideInEditor >= 1)`,
  constexpr std::array<uint8_t, 2> jump_gte = {0x0F, 0x8D};
  scen_editor_hook_[0].install((void*)0x4E0D8F, jump_gte.data(), jump_gte.size());
  scen_editor_hook_[1].install((void*)0x4E0AAF, jump_gte.data(), jump_gte.size());
  scen_editor_hook_[2].install((void*)0x4E0D8F, jump_gte.data(), jump_gte.size());
  // and `if (HideInEditor != 1)` to `if (HideInEditor < 1)`
  constexpr std::array<uint8_t, 1> short_jump_le = {0x7C};
  scen_editor_hook_[3].install((void*)0x7E509A, short_jump_le.data(), short_jump_le.size());
}
