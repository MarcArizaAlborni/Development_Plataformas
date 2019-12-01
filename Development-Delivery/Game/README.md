# Development_Plataformas
Developed for development delivery 2 CITM 2nd Course

Made by: 
- Adrian Mirabel (GitHub Link: [M1R4B3L](https://github.com/M1R4B3L))
- Marc Ariza (GitHub Link: [MarcArizaAlborni](https://github.com/MarcArizaAlborni))

## Download Game: 

- Last Release

 https://github.com/MarcArizaAlborni/Development_Plataformas/releases

- Github Repository:

 https://github.com/MarcArizaAlborni/Development_Plataformas


About the game:

This game is a 2D platformer game where you play as a character with directional movement (Left,Right,Up,Down). It also has a dash which allows the player to reach places which wouldn't be reachable with only a jump.
The game has currently 2 levels, they have their own tileset, enemies and music.
Flying enemies (2 of them) which can move freely (they are affected by walls and platforms) and will chase the player.
Ground enemies that will be in different platforms, once a player enters their Engage Distance, they will chase the player as long as they are in the same platform as their target. They dont jump/fly/fall.

Pathfinding has also been used for the enemies, this allows them to search for the player and reach them looking for the fastest path to the target.

Framerate can be capped/uncapped, it can also be done with vsync.?

Some of the Logic will be done depending on the framerate, others will be done independently of the framerate.





## INNOVATION

- CheckPoints to save progress that allow you to load in case you die.
- Ambient Music (depending on the area in a same level, different musics will be played).
- When the player starts combat (An enemy has detected the player nearby), a different music will be played.
- Teleporting platforms that allow the player to cover great distances and move around the map in an easier way.
- Dash has a cooldown that will not allow players to spam the move to fly across the level
- After Killing an enemy, the corpse will stay in the map.




## UML

UML map included to show the Entity System that has been used to create all entities in the game.

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
- Start Level 1 -> F1
  
- Start Level 2 -> F2

- Restart Current Level -> F3

- View Colliders/Logic -> F9

- God Mode -> F10 (ONCE IN GOD MODE YOU CAN'T EXIT IT)

- God Mode Move Left -> A

- God Mode Move Right -> D

- God Mode Move Up -> W

- God Mode Move Down -> S

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