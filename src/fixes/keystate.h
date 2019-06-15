#pragma once

/**
 * Masks the results of GetKeyState, GetKeyboardState, and GetAsyncKeyState so
 * they only contain the flags AoC expects, and no undefined bits.
 *
 * https://github.com/udf/aoe_keystate_fix
 */
class KeyState {
public:
  static void install();
};
