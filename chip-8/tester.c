#include <stdio.h>
#include <stdlib.h>

short getX(short opcode){
	return (opcode & 0x0F00) >> 8;
}

short getY(short opcode){
	return (opcode & 0x00F0)  >> 4;
}

//Made LLL so its not confused with NN.
short getLLL(short opcode){
	return (opcode & 0x0FFF);
}

short getNN(short opcode){
	return (opcode & 0x00FF);
}

//Made F to not confuse with other getLLL or getNN.
short getF(short opcode){
	return (opcode & 0x000F);
}

//Get Most Sig Bit for identification.
short getMSB(short opcode){
	return (opcode & 0xF000) >> 12;
}

int test(short (*foo) (short),int inputStartPosition, int inputEndPosition, int outputStartPosition, int outputEndPosition){
	char iString[7];
	char resString[7];	

	for(int i = 0; i < 0xffff; i++){
		
		sprintf(iString,"%#06x\n", i);
		sprintf(resString,"%#06x\n", (*foo)(i));

		for(int z = 0, j = inputStartPosition; z < 6; z++){
			if(z >= outputStartPosition && z <= outputEndPosition){
				if(resString[z] != iString[j]){
					printf("Error in matching\n");	
				}
				j++;
			}
			else{	
				if(resString[z] != '0' && (z==1 && resString[z] != 'x')){
					printf("Error in 0s or x \n");	
				}
			}
		}
	}
}

int main(){
	short (*foo) (short)= getX;
	test((*foo),3,3,5,5);
	return 0;
}
