#include "auto_hook.h"
#include <cassert>
#include <cstdio>
#include <windows.h>

#ifdef NDEBUG
#define dbg_print(...)
#else
#define dbg_print(...) printf("[community-patch] [hook] " __VA_ARGS__)
#endif

void AutoHook::write_bytes(void* ptr, const uint8_t* patch, size_t size) {
  assert(ptr != NULL);
  assert(patch != NULL);
  assert(size > 0);

  this->orig_data_ = std::make_unique<uint8_t[]>(size);

  DWORD old;
  DWORD tmp;
  if (!VirtualProtect(ptr, size, PAGE_EXECUTE_READWRITE, &old)) {
    dbg_print("Couldn't unprotect?! @ %p\n", ptr);
    this->uninstall();
    return;
  }
  std::memcpy(this->orig_data_.get(), ptr, size);
  std::memcpy(ptr, patch, size);
  VirtualProtect(ptr, size, old, &tmp);
}
