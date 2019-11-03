# Modding Features

## Civ-specific hill bonus
Player resources 211 and 212 contains modifiers for the hill attack bonus.
Resource 211 is a modifier for the attack multiplier when fighting downhill. The value of resource 211 is _added_ to the downhill attack multiplier. Set to 0, the default of 1.25× is used. Set to 0.25, the attack multiplier becomes 1.5×.
Resource 212 is a modifier for the attack multiplier when fighting uphill. The value of resource 212 is _added_ to the uphill attack multiplier. Set to 0, the default of 0.75× is used. Set to 0.25, the attack multiplier becomes 1×, eliminating the disadvantage.

## "Late take" store mode
Setting a unit resource storage's store mode to 16 acts like store mode 2, but waits for the unit's death animation has completed before resetting.

## Cuman Mercenaries
Player resources 214 and 215 influence the Cuman Mercenaries tech. When resource 214 reaches 0, the tech with ID 708 is automatically researched.

## Queueable Techs
To implement queueable techs, mod authors can use dummy units that cost no population room and trigger the tech when spawned. Each tech is tied to a player attribute so that they can be researched only once.
Additionally, a "queueable tech" flag can be set to enable the following behaviours:
- Change the "Creating Loom..." text during research to "Researching Loom...";
- Bypass the population cap, if a research is at the start of a building's production queue while the player has more units than the population limit for the game;
- Prevent showing the Gather Point button if all the units in a building are dummy units for queueable techs.
The "queueable tech" flag is the second bit in the "Hide In Editor" flag. Set the "Hide In Editor" value to 3 in the Advanced Genie Editor for all queueable tech units to opt in to these behaviours.
