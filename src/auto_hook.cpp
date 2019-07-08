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

  void* orig_data;
  if (size > 12) {
    this->big_orig_data_ = malloc(size);
    orig_data = this->big_orig_data_;
  } else {
    orig_data = this->small_orig_data_.data();
  }

  if (orig_data == nullptr)
    return;

  DWORD old;
  DWORD tmp;
  if (!VirtualProtect(ptr, size, PAGE_EXECUTE_READWRITE, &old)) {
    dbg_print("Couldn't unprotect?! @ %p\n", ptr);
    this->uninstall();
    return;
  }
  memcpy(orig_data, patch, size);
  memcpy(ptr, patch, size);
  VirtualProtect(ptr, size, old, &tmp);
}
