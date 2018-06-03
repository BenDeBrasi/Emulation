#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

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

regex_t regex1;
regex_t regex2;
regex_t regex3;
regex_t regex4;

int testWithValue;
int testWithoutValue;
int ansWithValue;
int ansWithoutValue;

char msgbuf[100];

int inputRegex;
int inputRegex1;
int outputRegex;
int outputRegex1;

int main(){
	
	testWithValue = regcomp(&regex1,"0x.[1-9]..",0);
	testWithoutValue = regcomp(&regex2,"0x.[0]..",0);
	ansWithValue = regcomp(&regex3,"0x000[^0]",0);
	ansWithoutValue = regcomp(&regex4,"0x000[0]",0);

	if(testWithValue){
		printf("Error compiling regex1.\n");
	}

	if(testWithoutValue){
		printf("Error compiling regex2.\n");
	}

	if(ansWithValue){
		printf("Error compiling regex3.\n");
	}

	if(ansWithoutValue){
		printf("Error compiling regex4.\n");
	}
		
	char iString[7];
	char resString[7];

	for(int i = 0; i < 0xFFFF; i++){
		sprintf(iString,"%#06x\n", i);
		sprintf(resString,"%#06x\n", getX(i));
		
		inputRegex = regexec(&regex1, iString,0, NULL, 0);
		inputRegex1 = regexec(&regex2, iString,0, NULL, 0);
		outputRegex = regexec(&regex3, resString,0, NULL, 0);
		outputRegex1 = regexec(&regex4, resString,0, NULL, 0);

		if((inputRegex != 0 && outputRegex != 0) || (inputRegex1 != 0 && outputRegex1 != 0)){
			printf("%d\n",i);
		}
		//Make these print messages more detailed if an error comes out
		else if (inputRegex == REG_NOMATCH || outputRegex == REG_NOMATCH || inputRegex1 == REG_NOMATCH || outputRegex1 == REG_NOMATCH){
			printf("Not a match\n");
		}else{
			printf("Error matching\n");
		}	
	}

	regfree(&regex1);
	regfree(&regex2);
	regfree(&regex3);
	regfree(&regex4);

	return 0;
}
