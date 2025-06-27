#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "Game.h"


bool fullscreen = true;




int main(int argc, char* args[]) {
	//Seed the random number generator with the current time so that it will generate different numbers every time the game is run.
	srand((unsigned)time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		std::cout << "Error: Couldn't initialize SDL Video or Audio = " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		//Setup the audio mixer.
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
			std::cout << "Error: Couldn't initialize Mix_OpenAudio = " << Mix_GetError() << std::endl;
		}
		else {
			Mix_AllocateChannels(32);

			//Output the name of the audio driver.
			std::cout << "Audio driver = " << SDL_GetCurrentAudioDriver() << std::endl;
		}


		//Create the window.
		SDL_Window* window = nullptr;
		if (fullscreen)
			window = SDL_CreateWindow("Falling Sand Platformer Game", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else
			window = SDL_CreateWindow("Falling Sand Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);

		if (window == nullptr) {
			std::cout << "Error: Couldn't create window = " << SDL_GetError() << std::endl;
			return 1;
		}
		else {
			//Create a renderer for GPU accelerated drawing.
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | 
				SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr) {
				std::cout << "Error: Couldn't create renderer = " << SDL_GetError() << std::endl;
				return 1;
			}
			else {
				//Set the mouse mode to relative to keep the mouse in the same position, deliver relative mouse 
				//movements, and hide the cursor.
				SDL_SetRelativeMouseMode(SDL_TRUE);

				//Ensure transparent graphics are drawn correctly.
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

				//Output the name of the render driver.
				SDL_RendererInfo rendererInfo;
				SDL_GetRendererInfo(renderer, &rendererInfo);
				std::cout << "Renderer = " << rendererInfo.name << std::endl;

				//Get the dimensions of the window.
				int windowWidth = 0, windowHeight = 0;
				SDL_GetWindowSize(window, &windowWidth, &windowHeight);

				//Start the game.
				Game game(window, renderer, windowWidth, windowHeight);

				//Clean up.
				SDL_DestroyRenderer(renderer);
			}

			//Clean up.
			SDL_DestroyWindow(window);
		}

		//Clean up.
		SDL_Quit();
	}
	return 0;
}