#ifndef blah
#define blah

#define SYS_MEM_SIZE 4096
#define CPU_REG_SIZE 16
#define SCREEN_SIZE 64*32
#define NUM_KEYS 16
#define STACK_SIZE 16

//current opcode
unsigned short opcode;

//System memory
unsigned char memory[4096];

//CPU registers
unsigned char V[16];

//Index/address register
unsigned short I;

//Program counter
unsigned short pc;

//pixel state array 1 if lighted 0 if dark
unsigned char screen[64][32];

//Timers 1 sound 1 gameplay
unsigned char delay_timer;
unsigned char sound_timer;

//Stack and stack pointer
unsigned short stack[16];
unsigned short sp;

//HEX based keypad
unsigned char key[16];

//Function pointers for opcodes. Second pointer for special case 0x8 opcodes.
void (*Chip8Table[16])();
void (*Chip8Auxiliary[19])();	

#endif
