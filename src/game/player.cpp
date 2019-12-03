#include "player.h"
#include "../auto_hook.h"
#include "../call_conventions.h"

std::array<Player::UnitCounts, 9> Player::unit_counts;
std::array<Player::UnitCounts, 9> Player::completed_unit_counts;

static void THISCALL(update_unit_count, Player* player, int32_t object_id, int32_t count) {
  player->updateUnitTypeCount(static_cast<uint16_t>(object_id), static_cast<uint16_t>(count));

  auto original = getMethod<void, Player*, int32_t, int32_t>(0x557DC0);
  original(player, object_id, count);
}

static std::array<CallHook, 10> update_unit_count_hook_;
void Player::install() {
  update_unit_count_hook_[0].install((void*)0x4B0FCC, (void*)update_unit_count);
  update_unit_count_hook_[1].install((void*)0x4B1341, (void*)update_unit_count);
  update_unit_count_hook_[2].install((void*)0x4D2035, (void*)update_unit_count);
  update_unit_count_hook_[3].install((void*)0x4D2069, (void*)update_unit_count);
  update_unit_count_hook_[4].install((void*)0x556773, (void*)update_unit_count);
  update_unit_count_hook_[5].install((void*)0x5567B7, (void*)update_unit_count);
  update_unit_count_hook_[6].install((void*)0x557B75, (void*)update_unit_count);
  update_unit_count_hook_[7].install((void*)0x557BE4, (void*)update_unit_count);
  update_unit_count_hook_[8].install((void*)0x7DCE1B, (void*)update_unit_count);
  update_unit_count_hook_[9].install((void*)0x7DCE47, (void*)update_unit_count);
}
