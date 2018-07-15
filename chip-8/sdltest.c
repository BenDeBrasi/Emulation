#include "SDL.h"

int main(){
	
	if(SDL_Init(SDL_INIT_EVERYTHING)){
		printf("SDL_Init() failed\n");
	}

	SDL_Quit();
}
