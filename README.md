# Double Dragon 3: The Rosetta Stone

This is my tribute to Double Dragon 3: The Rosetta Stone, released for Sega Genesis in 1993.

## PROJECT INFORMATION

Full project at github: [https://github.com/GuillemFP/DoubleDragon3](https://github.com/GuillemFP/DoubleDragon3) 

Project webpage: [https://guillemfp.github.io/DoubleDragon3/](https://guillemfp.github.io/DoubleDragon3/)  

Latest release (1.0): [https://github.com/GuillemFP/DoubleDragon3/releases/download/1.0/DoubleDragon3.zip](https://github.com/GuillemFP/DoubleDragon3/releases/download/1.0/DoubleDragon3.zip)  

*Author*: Guillem Ferré

Contact: guillemferre@gmail.com

License: GNU GENERAL PUBLIC LICENSE

## Gameplay video

<iframe width="560" height="315" src="https://www.youtube.com/embed/v-EPVxW6f4s" frameborder="0" allowfullscreen></iframe>
You can watch it directly on [youtube](https://www.youtube.com/watch?v=v-EPVxW6f4s).

Currently the game only features the first part of Stage 3, so only on type of enemy is present and there is no boss. Also, the store has not been included, so the playable characters don't have their full set of movements. Apart from that, the movement of the characters and enemies is fully implemented.

## Commands

The game supports up to two players. Each player can jump, punch, kick and perform a kick while in the air. Also, with the start button they are able to enter the game if they were defeated.

* **UP:** (Player 1: Up key / Player 2: W)
* **DOWN:** (Player 1: Down key / Player 2: S)
* **LEFT:** (Player 1: Left key / Player 2: A)
* **RIGHT:** (Player 1: Right key / Player 2: D)
* **START:** (Player 1: Return key / Player 2: Space)
* **PUNCH:** (Player 1: J / Player 2: C)
* **JUMP:** (Player 1: K / Player 2: V)
* **KICK:** (Player 1: L / Player 2: B)

The controls can be changed in the file "config.json", as well as other parameters that change the gameplay.

## Debug mode
Pressing F12 one can access debug mode, where others functions are availabe:

* F1: Show colliders: Player and enemy colliders are shown in red, while attacks are shown in blue. Green colliders are used for functionalities.
* F2: Debug camera: User can move camera around with the arrow keys of the numerical keyboard.
* F3: Positions: Print players positions (x,y,z).
* F4: God mode: Player is immune to enemy colisions.

## How to finish the game:

Currently there is only the first part of one stage. Only one type of enemy is accessible. Around the area up to 9 enemies will spawn. 
When all of them are defeated, the wooden door will open. The game will end when a player steps inside.
You can also lose if you spend all the initial 25 coins using continues or playing with other players.

## Acknowledgements

In this project I have used [parson](https://github.com/kgabis/parson) to handle the json document provided.
