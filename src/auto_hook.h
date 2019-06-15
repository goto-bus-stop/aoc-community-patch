#pragma once
#include "hook.h"

class AutoHook {
protected:
  hook_t hook_ = nullptr;

public:
  virtual ~AutoHook() { this->uninstall(); }

  void uninstall() {
    if (this->hook_ != nullptr) {
      revert_hook(this->hook_);
      this->hook_ = nullptr;
    }
  }
};

class JmpHook : public AutoHook {
public:
  JmpHook() = default;
  JmpHook(void* source_address, void* target_address) {
    this->install(source_address, target_address);
  }

  void install(void* source_address, void* target_address) {
    this->uninstall();
    this->hook_ = install_jmphook(source_address, target_address);
  }
};

class CallHook : public AutoHook {
public:
  CallHook() = default;
  CallHook(void* source_address, void* target_address) {
    this->install(source_address, target_address);
  }

  void install(void* source_address, void* target_address) {
    this->uninstall();
    this->hook_ = install_callhook(source_address, target_address);
  }
};

class VtblHook : public AutoHook {
public:
  VtblHook() = default;
  VtblHook(void* source_address, void* target_address) {
    this->install(source_address, target_address);
  }

  void install(void* source_address, void* target_address) {
    this->uninstall();
    this->hook_ = install_vtblhook(source_address, target_address);
  }
};

class BytesHook : public AutoHook {
public:
  BytesHook() = default;
  BytesHook(void* source_address, const void* buffer, size_t length) {
    this->install(source_address, buffer, length);
  }

  void install(void* source_address, const void* buffer, size_t length) {
    this->uninstall();
    this->hook_ = install_bytes(source_address, buffer, length);
  }
};
