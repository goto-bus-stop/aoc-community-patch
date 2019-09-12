#pragma once
#include "../auto_hook.h"

/// Include "player.h" to use this class's methods.
class Player;

class Game {
public:
  /// Get the current player.
  inline Player* player() const {
    auto original = getMethod<Player*, const Game*>(0x5E7560);
    return original(this);
  }

  inline const char* getString(int16_t lang_id) const {
    auto get_string = getMethod<const char*, const Game*, int16_t>(0x43A700);
    return get_string(this, lang_id);
  }

  inline void getString(int16_t lang_id, char* output, size_t size) const {
    auto get_string =
        getMethod<char*, const Game*, int16_t, char*, size_t>(0x5E4750);
    get_string(this, lang_id, output, size);
  }

  static inline Game* getInstance() {
    return *reinterpret_cast<Game**>(0x7912A0);
  }
};
