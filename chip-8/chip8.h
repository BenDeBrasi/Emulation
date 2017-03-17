void chip8::initialize(){
	//Initialize registers and memory once
	pc = 0x200; //Program counter starts at 0x200 cause thats were ROM is expected to be loaded
	opcode = 0; //Reset current opccode
	I = 0; //Reset index reg
	sp = 0; //Reset stack pointer
	
	//Clear display
	//Clear stack
	//Clear regs VO-VF
	//Clear memory

	//Load fontset
	for(int i = 0; i < 80; ++i){
		memory[i] = chip8_fontset[i];
		
	//Reset timers
	}

	//load program into mem using fopen in binary mode
	for(int i = 0; i < bufferSize; ++i)
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

	//Execute Opcode
	
	//Update timers
}
