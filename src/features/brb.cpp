#include "brb.h"
#include "../auto_hook.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

static CallHook hook;

int __cdecl read_hook(int32_t handle, char* buffer, uint32_t size) {
  auto original = (int __cdecl (*)(int32_t, char*, uint32_t))0x614235;

  auto result = original(handle, buffer, size);
  // the game reserves lots and lots of space for this so we can safely add to
  // the end
  strcat(buffer, "\r\n\r\n\r\n\r\nbrb");

  return result;
}

void BRB::install() { hook.install((void*)0x529ED3, (void*)read_hook); }
