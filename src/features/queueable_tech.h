#pragma once

/// Adds a "queueable tech" flag to unit data, allowing mods to implement tech
/// queueing using dummy units that trigger the tech when spawned, and then
/// immediately die.
class QueueableTech {
public:
  static void install();
};
