#pragma once
#include <string>
#include <vector>

struct UserPatchCiv {
  int32_t id;
  int16_t scout_unit;
  int16_t unique_unit;
  int16_t elite_unique_unit;
  int16_t unique_unit_line;
  int16_t unique_unit_upgrade;
  int16_t unique_tech;
  char name[32];
  char ai_const_name[32];
  char sound_file[64];
};

class UserPatch {
public:
  inline size_t numCivs() const { return *(int32_t*)0x7A5080;; }
  inline const UserPatchCiv* civs() const { return *(UserPatchCiv**)0x7A5090; }
  inline bool isModded() const { return strcmp(this->modShortName(), "age2_x1") != 0; }
  inline char* modName() const { return *(char**)0x7A5078; }
  inline char* modShortName() const { return *(char**)0x7A5058; }

  inline static UserPatch getInstance() {
    return UserPatch();
  }
};
