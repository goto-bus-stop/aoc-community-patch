#include "scenedit_pierce_armor.h"
#include "../auto_hook.h"
#include <array>

static BytesHook hook;
// Rewrite:
// push    ecx
// call    __ftol
// movsx   edx, ax
// push    edx
//
// To:
// call    __ftol
// movsx   edx, ax
// push    edx
// push    edx
static std::array<uint8_t, 10> patch = {0xE8, 0x92, 0xCE, 0x13, 0x00, 0x0F, 0xBF, 0xD0, 0x52, 0x52};

void ScenEditPierceArmor::install() {
  hook.install((void*)0x4D6B4D, patch.data(), patch.size());
}
