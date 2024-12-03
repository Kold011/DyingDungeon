# Dying Dungeon
Dying Dungeon is a 3D dungeon crawler turn based RPG where the player uses strategy and tactics to battle their way through a massive medieval-style dungeon. Players use team-building and skill to navigate the dungeon while defeating various enemies to earn experience and gear, allowing players to upgrade and customize their characters.

![image](https://github.com/user-attachments/assets/3d60a0e6-da0b-4fe8-991a-f46d57006557)

# Project Background
Dying Dungeon was developed as our final project capstone for Full Sail University. The project was built using Odyssey Engine, a custom game engine I developed for the project, over a period of 4 months (December 2019 - March 2020). All code was written during that time period and the only recent changes were to upgrade the build system to use Premake5 and support for C++17 and Visual Studio 2022.

# Supported Platforms
Currently Dying Dungeon is only supported on Windows and uses a visual studio 2022 build system through Premake5.

# Build Instructions
1. Run the Win-GenProjects.bat file located in the Scripts folder to generate the Visual Studio 2022 project and solution files.
2. Open the Dying Dungeon solution located at in repository root folder.
3. Build the solution in Debug or Relase.

# Controls
Dying Dungeon supports both keyboard controls and mouse input to interact with the game/

## Keyboard
* Use the arrow keys to cycle through which enemy to target
* Keys 1, 2, 3 and 4 will select the corresponding skill
* The enter key will use the currently selected skill on the target enemy
* The escape key will de-select the current skill

## Mouse
* Hovering over a skill will show the tooltip
* Clicking a skill will select the skill
* Clicking an enemy will use the skill on the selected enemy.
