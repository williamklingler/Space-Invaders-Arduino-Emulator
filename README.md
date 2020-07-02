# Space-Invaders-Arduino-Emulator
Recreated Space Invaders with custom bitmaps and graphics functions. Wrote to LCD pins from Arduino using digital pins. Created with C.

- See HelloWorld2.0.ino for the bulk of the source code
- The shooting of the player was controlled using the ultrasound sensor. It measured the distanced of the screen to the players hand. Once the player's hand was close over the screen, the player would shoot.
- The RGB light signified the status of the game. Inbetween every level, the light would turn white. When a player shot, it would turn red, and otherwise the light was blue.
- The rotor was used to change the horizontal position of the player on the screen.
- The bitmaps (the drawings of the spaceships and the players and the bullets) were created by me.

![space invaders image](https://raw.githubusercontent.com/williamklingler/Space-Invaders-Arduino-Emulator/master/space%20invaders%20arduino.jpg?raw=true)
