# File Formats

## SCX Mod Identifier
Scenario files now store the short name of the UserPatch mod that was used to create it. The identifier is stored in the 9th player name slot (0 is GAIA, 1-8 are players, 9 is the ID). The short name of the UserPatch mod is the name of the XML file, without the `.xml` extension. For example, a WololoKingdoms scenario will have "WK" in its 9th player name slot, and an Age of Chivalry scenario will have "aoc".
