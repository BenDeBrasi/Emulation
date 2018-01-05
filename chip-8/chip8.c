#include <stdio.h>
#include "chip8.h"
#include <stdlib.h>

signed char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
}

/* Key for Opcodes:

	NNN: address
	NN: 8-bit constant
	N: 4-bit constant
	X and Y: 4-bit register identifier
	PC : Program Counter
	I : 16bit register (For memory address) (Similar to void pointer)

*/

//Expand these 2 to include all 35 opcodes in both this file and .h. Determine which functions need to be in second.
void (*Chip8Table[17]) = 
{
	0NNN      , clear_display, return_sub, goto_NNN, call_NNN, VxIsNN, VxIsNotNN, VxIsNotVy, 
	cpuARITHMETIC, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
	cpuNULL
};

void (*Chip8Arithmetic[16]) = 
{
	cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, 
	cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL,
};

//Jump to a machine code routine at nnn. Opcode 0NNN
void 0NNN(unsigned short opcode){
	stack[sp] = pc;
	sp++;
	pc = memory[opcode | 0];
}

//Clears display. Opcode 00E0
void clear_display(char display[]){
	int i;
	for(i = 0; i < SCREEN_SIZE; i++){
		display[i] = 0;
	}

	//Use *j or j? Wouldn't NULL be wrong cause the random mem spot could be NULL or not be NULL?
	char *j;
	for(j = display; *j != NULL; j++){
		*j = 0;	
	}
}	

//Returns from a subroutine. Opcode 00EE.
void return_sub(){
	pc = stack[sp];
	sp--;	
}
//Jumps to location nnn. Opcode 1NNN.
void goto_NNN(){
	//Creating num 0x1000 by shifting 0x0001 12 times then using ^ to toggle that bit only.
	pc = memory[opcode ^ (0x0001 << 12)];
}
	
//Calls subroutine at nnn. Opcode 2NNN.
void call_NNN(){
	stack[sp] = pc;
	sp++;
	//Creating num 0x2000 by shifting 0x0001 13 times then using ^ to toggle that bit only.
	pc = opcode ^ (0x0001 << 13);
}

//Checking if stack pointer is at limit? Increment sp before or after placing pc in it?

//Skips next instruction if Vx = NN. Opcode 3XNN.
void VxIsNN(){
	if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
		pc+=4;
	else
		pc+=2;
}

//Skips next instruction if Vx != NN. Opcode 4xNN.
void VxIsNotNN(){		
	if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
		pc+=4;
	else
		pc+=2;
}


//Skips next instruction if Vx = Vy. Opcode 5xY0.
void VxIsVy(){	
	if(V[(opcode & 0x0F00) >> 8] == V[((opcode & 0x00F0) >> 4)])
		pc+=4;
	else
		pc+=2;
}

//Sets Vx = NN. Opcode 6XNN.
void NNToVx(){
	V[((opcode & 0x0F00) >> 8)] = (opcode & 0x00FF);
}

//Adds NN to Vx. Opcode 7XNN.
void NNAddedToVx(){
	V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
}

//Sets Vx to the value of Vy. Opcode 8XY0.
void VxToVy(){
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
}

//Sets Vx to Vx OR Vy. Opcode 8XY1.
void VxToOR(){
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4]; 						
}

//Sets Vx to Vx AND Vy. Opcode 8XY2. 
void VxToAND(){
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4]; 
}

//Sets Vx to Vx XOR Vy. Opcode 8XY3. 
void VxToAND(){
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4]; 
}

//Adds Vy and Vx to Vx keeping only last 8 bits. If result is > 255 (8 bits) set Vf to 1. Otherwise 0. Opcode 8XY4.
void VxVyAddedToVxCarry(){
	unsigned short num = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
	//May have to change to bit comparasion. num > 0x00FF?
	V[(opcode & 0x0F00) >> 8] = (num & 0x00FF);
	
	if(num > 255){
		V[0x000F] = 1;
	}
	
	else{
		V[0x000F] = 0;
	}
}


//Subtracts Vy from Vx to Vx keeping only last 8 bits. If result is > 255 (8 bits) set Vf to 1. Otherwise 0. Opcode 8XY5.
void VyVxSubtractedToVxCarry(){
	unsigned short num = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
	//Online doc says if Vx > Vy then Vf = 1, but shouldn't it be Vy > Vx?
	V[(opcode & 0x0F00) >> 8] = (num & 0x00FF);
	if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){
		V[0x000F] = 1;
	}
	
	else{
		V[0x000F] = 0;
	}
}

//PROBLEM: 2 documentation sources seem to be saying different things about functionality. Must research opcode further.
//Set Vf as least sig bit in Vy. Shifts Vy right by 1 (divides by 2) and sets Vx as result. Opcode 8XY6.
void VyDiv2IntoVxCarry(){
	V[0x000F] = V[(opcode & 0x00F0) >> 4] & 0x0001;
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] >> 1;	
}


void chip8::initialize(){
	//Initialize registers and memory once
	pc = 0x200; //Program counter starts at 0x200 cause thats were ROM is expected to be loaded
	opcode = 0; //Reset current opccode
	I = 0; //Reset index reg
	sp = 0; //Reset stack pointer
	
	//Clear display
	int i;
	for(i = 0; i < SCREEN_SIZE; i++){
		screen[i] = 0;
	}

	//Clear stack
	for(i = 0; i < STACK_SIZE; i++){
		stack[i] = 0;
	}
	
	//Clear registers
	for(i = 0; i < CPU_REG_SIZE; i++){
		V[i] = 0;
	}

	//Clear memory
	for(i = 0; i < SYS_MEM_SIZE; i++){
		memory[i] = 0;
	}


	//Load fontset
	for(i = 0; i < 80; ++i){
		memory[i] = chip8_fontset[i];
	}

	//Reset timers
	delay_timer = 0;
	sound_timer = 0;

	//load program into mem using fopen in binary mode
	for(i = 0; i < bufferSize; ++i)
		memory[i+512] = buffer[i]
}

void chip8::emulateCycle(){

	//Fetch Opcode
	opcode = memory[pc] << 8 | memory[pc+1];

	//Decode Opcode

	switch(opcode & 0xF000){
		case 0xA00: //ANNN: Sets I to the address NNN
			//Execute opcode
			I = opcode & 0x0FFF;
			pc+=2;
		break;

		//More opcodes 

		default:
		printf("Unknown opcode: 0x%X\n", opcode);
	}
	
	//Update timers
	if(delay_timer > 0)
		--delay_timer;

	if(sound_timer > 0){
		printf("BEEP!\n");
		--sound_timer;
	}

	//Some opcodes cant be decoded using the first four bits so we need another switch statement

	switch(opcode & 0xF000){
		case 0x000:
			switch(opcode & 0x000F){
				case 0x0000: //0x00E0: clears the screen
					//Execture opcode
				break;
		case 0x000E: //0xOOEE: Returns from subroutine
				//Execute opcode
		break;

		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
	}
	break;
	//more opcodes

	case 0x2000:
		stack[sp] = pc;
		++sp;
		pc = opcode & 0x0FFF;
	break;

	case 0x0004:
		if(V[(opcode & 0x00F0) >> 4] > (0xFF - V(opcode & 0x0F00) >>  8]))
			V[0xF] = 1; //carry
		else
			V[0xF] = 0;
		V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
		pc += 2;
	break;

	case 0x0033:
		memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
		memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
		memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
		pc +=2;
	break;
	
	case 0xD000:
	{
		unsigned short x = V[(opcode & 0x0F00) >> 8];
		unsigned short y = V[(opcode & 0x00F0) >> 4];
		unsigned short height = opcode & 0x000F;
		unsigned short pixel;

		V[0xF] = 0;
		for (int yline = 0; yline < height; yline++){
			pixel = memory[I + yline];
			for(int xline = 0; xline < 8; xline++){
				if(gfx[( x + xline + ((y +yline) * 64))] == 1){
					V[0xF] = 1;
				gfx[x + xline + ((y + yline) * 64)] ^= 1;
				}
			}
		}
	
		drawFlag = true;
		pc += 2;
	}
	break;

	case 0xE000:
		switch(opcode & 0x00FF){
			// EX9E: Skips the next instruction 
			// if the key stored in VX is pressed
			case 0x009E:
				if(key[V[(opcode & 0x0F00) >> 8]] != 0)
					pc += 4;
				else
					pc += 2;
			break;
	//Execute Opcode
	
	//Update timers
}
