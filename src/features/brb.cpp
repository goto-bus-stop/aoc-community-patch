#include "brb.h"
#include "../auto_hook.h"
#include "../call_conventions.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

static CallHook hook;

int CDECL(read_hook, int32_t handle, char* buffer, uint32_t size) {
  auto original = CDECL_PTR(int, int32_t, char*, uint32_t) 0x614235;

  auto result = original(handle, buffer, size);
  // the game reserves lots and lots of space for this so we can safely add to
  // the end
  auto buffer_size = (size + 1) * 0x400;
  strcat(buffer, "\r\n\r\n\r\n\r\nbrb");

  return result;
}

void BRB::install() { hook.install((void*)0x529ED3, (void*)read_hook); }
