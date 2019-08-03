#pragma once

/**
 * Adds the necessary builtin features for the Cuman Mercenaries tech.
 *
 * Player attribute 214 contains the number of mercenaries that can be made.
 * Player attribute 215 contains the number of mercenaries that have been made.
 *
 * When 214 reaches 0, tech 708 is triggered.
 *
 * The data files can specify that 708 should disable the mercenary training
 * button for that player.
 */

class Mercenaries {
public:
  static void install();
};
