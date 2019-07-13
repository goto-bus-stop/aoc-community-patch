#include "keystate.h"
#include "../auto_hook.h"
#include <cstdint>
#include <windows.h>

#ifdef NDEBUG
#define dbg_print(...)
#else
#define dbg_print(...) printf("[community-patch] [keystate] " __VA_ARGS__)
#endif

static BytesHook key_state_hook_;
static BytesHook keyboard_state_hook_;
static BytesHook async_key_state_hook_;

uint16_t WINAPI get_key_state(int32_t virt_key) {
  auto state = static_cast<uint16_t>(GetKeyState(virt_key));
  dbg_print("get_key_state(%d) = %x -> %x\n", virt_key, state, state & 0x8001);
  // Only the low-order and high-order bits are defined.
  return state & 0x8001;
}

bool WINAPI get_keyboard_state(uint8_t* states) {
  auto res = GetKeyboardState(states);
  if (res)
    return res;

  for (auto i = 0; i < 256; i += 1) {
    // Only the low-order and high-order bits are defined.
    states[i] &= 0x81;
  }

  return false;
}

uint16_t WINAPI get_async_key_state(int32_t virt_key) {
  auto state = static_cast<uint16_t>(GetAsyncKeyState(virt_key));
  dbg_print("get_async_key_state(%d) = %x -> %x\n", virt_key, state,
            state & 0x8001);
  // Only the low-order and high-order bits are defined.
  return state & 0x8001;
}

static void* ptr_key_state;
static void* ptr_keyboard_state;
static void* ptr_async_key_state;

void KeyState::install() {
  ptr_key_state = (void*)get_key_state;
  ptr_keyboard_state = (void*)get_keyboard_state;
  ptr_async_key_state = (void*)get_async_key_state;

  key_state_hook_.install((void*)0x63534C, (void*)&ptr_key_state, 4);
  keyboard_state_hook_.install((void*)0x635358, (void*)&ptr_keyboard_state, 4);
  async_key_state_hook_.install((void*)0x635390, (void*)&ptr_async_key_state,
                                4);
}
