#pragma once

/// Add a mod identifier to scenario files. This writes the same identifier that
/// UserPatch uses for the mod XML file. For example, for WololoKingdoms, "WK"
/// is used.
///
/// The mod identifier is written in the 9th player name slot in the SCX file.
class SCXModIdentifier {
public:
  static void install();
};
