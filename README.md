# Development_Plataformas
Developed for development delivery 3 CITM 2nd Course

Made by: 
- Adrian Mirabel (GitHub Link: [M1R4B3L](https://github.com/M1R4B3L))
- Marc Ariza (GitHub Link: [MarcArizaAlborni](https://github.com/MarcArizaAlborni))

## Download Game: 

- Last Release

 https://github.com/MarcArizaAlborni/Development_Plataformas/releases

- Github Repository:

 https://github.com/MarcArizaAlborni/Development_Plataformas

- Webpage

 https://arizamarc1999.wixsite.com/projectceta





## About the game:

 OLD CONTENT

Game Developed in C++ using SDL Libraries and Tiled to create maps. Brofiler to keep track of performance.

This game is a 2D platformer game where you play as a character with directional movement (Left,Right,Up,Down). It also has a dash which allows the player to reach places which wouldn't be reachable with only a jump.
The game has currently 2 levels, they have their own tileset, enemies and music.
Flying enemies (2 of them) which can move freely (they are affected by walls and platforms) and will chase the player.
Ground enemies that will be in different platforms, once a player enters their Engage Distance, they will chase the player as long as they are in the same platform as their target. They dont jump/fly/fall.

Pathfinding has also been used for the enemies, this allows them to search for the player and reach them looking for the fastest path to the target.

Framerate can be capped/uncapped, it can also be done with vsync.?

Some of the Logic will be done depending on the framerate, others will be done independently of the framerate.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 NEW CONTENT (3rd Delivery)

A new UI system has been developed, it has all common UI items such as buttons, sliders and Checkbox. Each button will have an audio effect to know when they have been clicked, Sliders will move with the cursor dragging them,
and checkboxes will change sprite depending in their state.

The UI system has 4 main menus:
- MainMenu which is the one that opens when the game is executed. This menu has play/continue buttons to start a new game or to continue the one you left before. It also has a button to enter the settings menu, and 
  buttons that link to the game Webpage, to the credits section and to exit the game.

- Settings Menu is where the main options for the game are, there are 2 sliders to modify Music and FX volume and  a mute checkbox for Music and FX.

- Ingame Menu: Its similar to the Settings Menu, has mute options for Music and FX, a back to MainMenu button, an exit menu button to keep playing, and Load/Save buttons to save progress while playing. 
  All debugging functionalities are still available

- Player UI, the player now has a limited amount of health, shown with 2 different sprites (Hearts and Character Model), which will change depending in the amount of Health remaining, there is also a UI sprite to
  keep track of the amount of maps that have been collected.

- A console has also been implemented (for debugging), this console works as a visual menu where it will perform the action told by the player by using a menu. The console also gives warnings before performing
  certain actions and keeps track of the 5 last commands that have been performed. 
   
    - LIST OF COMANDS: KILL PLAYER, -1 HP, +1 HP, FULL HEAL, GODMODE, FPS NO CAP, FPS CAP AT 30, FPS CAP AT 60, VSYNC ON, VSYNC OFF, RESET ENTITIES, LOAD MAP 1, LOAD MAP 2, COLLECT MAP +1, COLLECT MAP -1.


## WARNING

 If right after starting the game, the player gets stuck, RESTART the game and eveything will be fine.



## UML

UML map included to show the UI System that has been used to create the entire UI in the game.

## Controls:
- Left -> A    
- Right -> D
- Jump -> Space
- Dash -> U (Dash Available when the player falls) (With Cooldown)
- Activate Teleporter Platform -> I

## Save/Load the location of the character 
- Save Game -> F5

- Load Game -> F6

## Debuging functions:  
  

- Restart Current Level -> F3

- View Colliders/Logic -> F9

- God Mode -> F10 (ONCE IN GOD MODE YOU CAN'T EXIT IT)

- God Mode Move Left -> A

- God Mode Move Right -> D

- God Mode Move Up -> W

- God Mode Move Down -> S

## UI Controls:

- ESC to Open Ingame Menu

- o to Open Console

- Move Sliders to change values

- Click on Buttons to interact

- Click on Checkbox to interact

## Credits:

- ART:
        
        https://opengameart.org/content/generic-platformer-tiles

        https://anokolisa.itch.io/pocket-pack-black-temple
        
        https://itch.io (Enemy Spritesheets)


   
- AUDIO:

        https://opengameart.org/content/platformer-game-music-pack
        The Witcher 3 SoundTrack





## License:

MIT License

Copyright (c) [2019] [Marc Ariza, Adrian Mirabel]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
