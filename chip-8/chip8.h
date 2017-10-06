#include<stdio.h>
#include<stdlib.h>

int main(){
	
	//current opcode
	unsigned short curr_opcode;
	
	//System memory
	unsigned char memory[4096];
	
	//CPU registers
	unsigned char V[16];
	
	//Index/address register
	unsigned short I;

	//Program counter
	unsigned short pc;

	//pixel state array 1 if lighted 0 if dark
	unsigned char screen[64 *32];

	//Timers 1 sound 1 gameplay
	unsigned char delay_timer;
	unsigned char sound_timer;

	//Stack and stack pointer
	unsigned short stack[16];
	unsigned short sp;
	
	//HEX based keypad
	unsigned char key[16];

	
}
