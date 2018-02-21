#include <stdio.h>
#include <stdlib.h>
// #include OpenGL graphics and input
#include "chip8.h" //CPU implementation

chip8 myChip8;

int main(int argc, char **argv){
	
	//Set up render system and register input callbacks
	setupGraphics();
	setupInput();

	//Initialize the Chip8 system and load the game into the memory
	initialize();
	loadGame("pong");

	//Emulation loop
	for(;;){
		
		//Emulate one cycle
		emulateCycle();

		//If the draw flag is set, update the screen
		if(myChip8.drawFlag)
			drawGraphics();

		//Store key press state (Press and release)
		myChip8.setKeys();
	
	}
		return 0;
}
