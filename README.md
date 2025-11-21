# ![Alien Swarm: Global Offensive](.github-assets/logo.png)

Alien Swarm: Global Offensive is an attempt to make a CS:GO modding SDK on the Alien Swarm. Based on [Enhanced Source](https://github.com/Enhanced-Source-Project/Enhanced-Source).

Partially used code from:
- [Emulsion SDK](https://github.com/theKlaxon/emulsion-sdk)
- [Enhanced Source](https://github.com/Enhanced-Source-Project/Enhanced-Source) (literally entire base of this shit)

## Setting up the project
- Firstly install Visual Studio 2019 or its build tools using the Visual Studio installer
- Windows 10 SDK (yes, it is compatible with it)
- CS:GO and Enhanced Source game files

Firstly copy the CS:GO and Enhanced Source game files inside the `//game/` folder (DO NOT REPLACE ANYTHING), then go to `//src/` and run `CreateSolution.bat`, it will generate a solution for the modification. You can add [other games](#supported-games), but the default project is HL2.
Open the solution, compile it, and run!

Currently there are a bunch of bugs, please make a GitHub issue to report them.

## Supported games
Majority of these games are untested.

- `DEFERRED` - [Alien Swarm Deferred](https://developer.valvesoftware.com/wiki/Alien_Swarm_Deferred)
- `TEMPLATE` - Custom mod template
- `PORTAL`/`PORTAL_TEMPLATE` - Portal
- `HL2`/`HL2_TEMPLATE` - Half-Life 2
- `HL2_EPISODIC` - Half-Life 2 Episode (1/2)
- `VECTRONIC` - The old [mod version](https://www.moddb.com/mods/punt) of [PUNT](https://www.moddb.com/games/punt)

## Currently supported stuff
|      OS    | Supported?   |
|------------|--------------|
|Windows (10)| Yes          |
|Windows (11)| Yes          |
| Linux      | Yes (requires Case Sensitive fixes) |