#include "SDL2/SDL.h"

int main(){
	
	if(SDL_Init(SDL_INIT_EVERYTHING)){
		printf("SDL_Init() failed\n");
		return 1;
	}

	//load WAV file
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	SDL_LoadWAV("beep-03.wav", &wavSpec, &wavBuffer, &wavLength);
	
	//open audio device
	SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL,0,&wavSpec,NULL,0);
	
	//play audio
	int success = SDL_QueueAudio(deviceID,wavBuffer,wavLength);
	SDL_PauseAudioDevice(deviceID,0);

	//keep app running long enough to hear sound
	SDL_Delay(3000);

	//clean up
	SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(wavBuffer);

	SDL_Quit();
	return 0;
}
