#include "config.h"
#include <cstdio>
#include <cstring>
#include <ini.h>
#include <charconv>

Config config;
Config* Config::getInstance() { return &config; }

int32_t Config::onIniKey(const char* section, const char* key, const char* value) {
  if (stricmp(section, "behavior") == 0) {
    if (stricmp(key, "selectionsize") == 0) {
      std::from_chars(value, value + strlen(value), this->max_selection_size);
    }
  }
  return 1;
}

static int32_t handler(void* user, const char* section, const char* key, const char* value) {
  return reinterpret_cast<Config*>(user)->onIniKey(section, key, value);
}

void Config::load(const char* filename) {
  auto fd = fopen(filename, "r");
  if (fd == nullptr) return;
  auto ini = ini_parse_file(fd, handler, this);
}
