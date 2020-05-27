#pragma once
#include <cstdint>

class Config {
private:
  const char* load_error_ = nullptr;
public:
  int32_t max_selection_size = 40;
  bool use_new_resource_panel = false;

  int32_t onIniKey(const char* section, const char* key, const char* value);
  void load(const char* filename);
  void reportError();

  static Config* getInstance();
};
