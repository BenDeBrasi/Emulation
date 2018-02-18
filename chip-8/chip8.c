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

//Functions for commonly used values embedded in opcodes. Of the form AXAA, AAYA, ALLL, AANN and AAAN respectively.


short getX(opcode){
	return (opcode & 0x0F00) >> 8;
}

short getY(opcode){
	return (opcode & 0x00F0)  >> 4;
}


//Made LLL so its not confused with NN.
short getLLL(opcode){
	return (opcode & 0x0FFF);
}

short getNN(opcode){
	return (opcode & 0x00FF);
}

//Made F to not confuse with other getLLL or getNN.
short getF(opcode){
	return (opcode & 0x000F);
}


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

	pc+=2;
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
	if( V[getX(opcode)] == getNNN(opcode) )
		pc+=4;
	else
		pc+=2;
}

//Skips next instruction if Vx != NN. Opcode 4xNN.
void VxIsNotNN(){		
	if( V[getX(opcode)] != getNN(opcode) )
		pc+=4;
	else
		pc+=2;
}


//Skips next instruction if Vx = Vy. Opcode 5xY0.
void VxIsVy(){	
	if( V[getX(opcode)] == V[getY(opcode)] )
		pc+=4;
	else
		pc+=2;
}

//Sets Vx = NN. Opcode 6XNN.
void NNToVx(){
	V[getX(opcode)] = getNN(opcode);
	pc += 2;
}

//Adds NN to Vx. Opcode 7XNN.
void NNAddedToVx(){
	V[getX(opcode)] += getNN(opcode);
	pc+=2;
}

//Sets Vx to the value of Vy. Opcode 8XY0.
void VxToVy(){
	V[getX(opcode)] = V[getY(opcode)];
	pc+=2;
}

//Sets Vx to Vx OR Vy. Opcode 8XY1.
void VxToOR(){
	V[getX(opcode)] = V[getX(opcode)] | V[getY(opcode)]; 						
	pc+=2;
}

//Sets Vx to Vx AND Vy. Opcode 8XY2. 
void VxToAND(){
	V[getX(opcode)] = V[getX(opcode)] & V[getY(opcode)]; 
	pc+=2;
}

//Sets Vx to Vx XOR Vy. Opcode 8XY3. 
void VxToAND(){
	V[getX(opcode)] = V[getX(opcode)] ^ V[getY(opcode)]; 
	pc+=2;
}

//Adds Vy and Vx to Vx keeping only last 8 bits. If result is > 255 (8 bits) set Vf to 1. Otherwise 0. Opcode 8XY4.
void VxVyAddedToVxCarry(){
	unsigned short num = V[getX(opcode)] + V[getY(opcode)];
	//May have to change to bit comparasion. num > 0x00FF?
	V[getX(opcode)] = (num & 0x00FF);
	
	if(num > 255){
		V[0x000F] = 1;
	}
	
	else{
		V[0x000F] = 0;
	}

	pc+=2;
}


//Subtracts Vy from Vx to Vx keeping only last 8 bits. If result is > 255 (8 bits) set Vf to 1. Otherwise 0. Opcode 8XY5.
void VyVxSubtractedToVxCarry(){
	unsigned short num = V[getX(opcode)] - V[getY(opcode)];
	//Online doc says if Vx > Vy then Vf = 1, but shouldn't it be Vy > Vx?
	V[getX(opcode)] = (num & 0x00FF);
	if(V[getX(opcode)] > V[getY(opcode)]){
		V[0x000F] = 1;
	}
	
	else{
		V[0x000F] = 0;
	}

	pc+=2;
}

//Set Vf as least sig bit in Vy. Shifts Vy right by 1 (divides by 2) and sets Vx as result. Opcode 8XY6.
void VyDiv2IntoVxCarry(){
	V[0x000F] = V[getY(opcode)] & 0x0001;
	V[getX(opcode)] = V[getY(opcode)] >> 1;	
	pc+=2;
}

//Sets Vx = Vy - Vx. If Vy > Vx then Vf = 1, o.w. 0. Opcode 8XY7.
void Op_8XY7(){
	
	if(V[getY(opcode)] > V[getX(opcode)])
		V[0x000F] = 1;
	else
		V[0x000F] = 0;

	V[getX(opcode)] = V[getY(opcode)] - V[getX(opcode)];
	pc+=2;
}

//If most sig fig bit of Vx is 1, then Vf is set to 1 o.w. 0. Then Vx *= 2. Opcode 8XYE.
void Op_8XYE(){
	
	if(V[getX(opcode)] & 0x1000 == 1)
		V[0x000F] = 1;
	else
		V[0x000F] = 0;

	V[getX(opcode)] *= 2;
	pc+=2;
}

//Skips next instruction if Vx != Vy. Opcode 9XY0.
void Op_9XY0(){
	if(V[getX(opcode)] == V[getY(opcode)])
		pc+=4;
	else
		pc+=2;
}

//Sets value of register I to nnn. Opcode Annn.
void Op_Annn(){
	I = getLLL(opcode);
	pc += 2;
}

//Jumps pc =  nnn + v0. Opcode BNNN.
void Op_Bnnn(){
	pc = getLLL(opcode) +V[0];
}

//Set Vx = random byte AND kk. Opcode CXNN.
void Op_CXNN(){
	V[getX(opcode)] = ((rand() % 255) & getNN(opcode));
	pc +=2;
}

//Draw a sprite at coordinate (Vx, Vy) with width 8 and N height. Write from reg I. Vf is set to if pixel erases. Opcode DXYN.
void Op_DXYN(){
	

	//Is is memory[I] or I itself that I'm getting data from?
	//I is short (2 bytes) and pixel is char (1 byte). 
	
	char *pixel;
	short *mem_start = memory[I];
	
	Vx = getX(opcode);
	Vy = getY(opcode);
	height = opcode & 0x000F;

	int i;
	int j;

	for(i = 0; i < height; i++){
		//always 8 pixels width
		for(j = 0; j < 8; j++){
			
			pixel = screen[Vx][Vy];
			
		
			//To set VF 1 if the and of I and current 8 pixels (0s out all mismatches doesnt match then pixel had a 1 pixel that was flipped.
			if ((mem_start & pixel) != 0){
				V[0x000F] = 1;
			}
			
			if(j%2==0){
				*pixel = *pixel ^ ((*mem_start & 0x00F0) >> 4);
			}
			else{
				*pixel = *pixel ^ (*mem_start & 0x000F);
				*mem_start++;
			}

			Vx++;
			Vy++;

			if (Vx == 64){
				Vx = 0;
			}

			if (Vy ==  32){
				Vy = 0;
			}
		}
	}

	pc+=2;
}

//If key being held down from player == Vx, skip next instruction (pc+=4)
void EX9E(){
	if(key[getX(opcode)] == 0)
		pc+=2
	else
		pc+=4
}

//If key Vx is not being held down skip next instruction (pc+=4)
void EXA1(){
	if(key[getX(opcode)] == 0)
		pc+=4
	else
		pc+=2
}

//Sets Vx to delay_timer
void FX07(){
	getX(opcode) = delay_timer;
	pc+=2;
}


//Wait until a key is pressed and then store it in Vx. 
//Have to figure out how to deal with key presses.
void FX0A(){
	while(key not pressed){
		getX(opcode) = key
	}
	pc+=2;
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

	//Execute Opcode
	
	//Update timers
}
