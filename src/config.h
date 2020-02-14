#pragma once
#include <cstdint>

class Config {
public:
  int32_t max_selection_size = 40;

  int32_t onIniKey(const char* section, const char* key, const char* value);
  void load(const char* filename);

  static Config* getInstance();
};
