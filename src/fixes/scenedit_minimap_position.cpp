#include "scenedit_minimap_position.h"
#include "../auto_hook.h"
#include <array>

static BytesHook hook;
// asm bytes for PUSH 20
static std::array<uint8_t, 2> new_arg = {0x6A, 20};

void ScenEditMinimapPosition::install() {
  hook.install((void *)0x4EB4C9, new_arg.data(), new_arg.size());
}
