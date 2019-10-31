#pragma once
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>

template <typename ReturnType, typename ThisArg, typename... Args>
inline auto getMethod(size_t address) {
#ifdef _MSC_VER
  auto fn = reinterpret_cast<ReturnType(__fastcall*)(ThisArg, void*, Args...)>(
      address);
  return [fn](ThisArg self, Args... args) -> ReturnType {
    return fn(self, nullptr, std::forward<Args>(args)...);
  };
#else
  // Simply doing `auto fn = reinterpret_cast<>(address)` loses the __thiscall
  // declaration, so we have to trick GCC into accepting it.
  ReturnType __thiscall (*fn)(ThisArg, Args...) = nullptr;
  fn = reinterpret_cast<decltype(fn)>(address);
  return fn;
#endif
}

class AutoHook {
protected:
  void* ptr_ = nullptr;
  size_t orig_size_ = 0;
  union {
    void* big_orig_data_;
    std::array<uint8_t, 12> small_orig_data_;
  };

  void write_bytes(void* at, const uint8_t* patch, size_t size);

public:
  virtual ~AutoHook() { this->uninstall(); }

  void uninstall() {
    if (this->ptr_ != nullptr) {
      if (this->orig_size_ > 12) {
        memcpy(this->ptr_, this->big_orig_data_, this->orig_size_);
        free(this->big_orig_data_);
      } else {
        memcpy(this->ptr_, this->small_orig_data_.data(), this->orig_size_);
      }
      this->ptr_ = nullptr;
      this->orig_size_ = 0;
      this->small_orig_data_.fill(0);
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
    std::array<uint8_t, 5> bytes = {0xE9, 0, 0, 0, 0};
    int32_t offset = reinterpret_cast<int32_t>(target_address) -
                     (reinterpret_cast<int32_t>(source_address) + 5);
    memcpy(&bytes[1], &offset, sizeof(offset));
    this->write_bytes(source_address, bytes.data(), bytes.size());
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
    std::array<uint8_t, 5> bytes = {0xE8, 0, 0, 0, 0};
    int32_t offset = reinterpret_cast<int32_t>(target_address) -
                     (reinterpret_cast<int32_t>(source_address) + 5);
    memcpy(&bytes[1], &offset, sizeof(offset));
    this->write_bytes(source_address, bytes.data(), bytes.size());
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
    this->write_bytes(source_address,
                      reinterpret_cast<const uint8_t*>(&target_address), 4);
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
    this->write_bytes(source_address, reinterpret_cast<const uint8_t*>(buffer),
                      length);
  }
};
