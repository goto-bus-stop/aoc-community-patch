#include "config.h"
#include "auto_hook.h"
#include <cstdio>
#include <cstring>
#include <ini.h>
#include <charconv>

static char load_error_buffer[1000];

Config config;
Config* Config::getInstance() { return &config; }

int32_t Config::onIniKey(const char* section, const char* key, const char* value) {
  printf("Config::onIniKey(%s, %s, %s)\n", section, key, value);

  if (stricmp(section, "behavior") == 0) {
    if (stricmp(key, "selectionsize") == 0) {
      std::from_chars(value, value + strlen(value), this->max_selection_size);
    }
  }
  if (stricmp(section, "resourcepanel") == 0) {
    if (stricmp(key, "enabled") == 0) {
      this->use_new_resource_panel = stricmp(value, "1") == 0;
    }
  }
  return 1;
}

static int32_t handler(void* user, const char* section, const char* key, const char* value) {
  return reinterpret_cast<Config*>(user)->onIniKey(section, key, value);
}

void Config::load(const char* filename) {
  printf("Config::load(%s)\n", filename);
  auto result = ini_parse(filename, handler, this);
  if (result != 0) {
    snprintf(load_error_buffer, sizeof(load_error_buffer), "Failed to parse '%s'. Please report this to the mod author.", filename);
    this->load_error_ = load_error_buffer;
  }
}

void Config::reportError() {
  if (this->load_error_ == nullptr) {
    return;
  }

  auto panel_system = 0x7861B8;
  auto current_panel = *reinterpret_cast<void**>(panel_system + 0x10);
  auto show_dialog = getMethod<void, void*, const char*, const char*, int32_t, int32_t, bool>(0x55F4F0);
  show_dialog(current_panel, this->load_error_, nullptr, 450, 180, 0);
  this->load_error_ = nullptr;
}
