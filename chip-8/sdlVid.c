#include "SDL2/SDL.h"
#include <stdio.h>

int main(){
	
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		printf("Init error\n");
		return 1;
	}
	
	SDL_Window *window;
	window = SDL_CreateWindow(
		"SDL2 window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	
	if (window == NULL){
		printf("Screen error\n");
		return 1;
	}
	
	SDL_Delay(3000);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
