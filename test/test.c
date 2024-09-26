#include "include/SDL2/SDL.h"

#undef main

int main(void){
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_bool stop = SDL_FALSE;
	SDL_Rect dstrect;
	int x = 720;
	int k = 0;
	dstrect.x = 0;
	dstrect.y = 173;
	dstrect.w = 248;
	dstrect.h = 135;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	surface = SDL_LoadBMP("horse.bmp");
	texture = (SDL_Texture *)SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);


	while (!stop) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type){
				case SDL_QUIT:
					stop = SDL_TRUE;
					break;
			}
		}
		if (k == 0){
			if (x < -268){
				x = 720;
			}
			else {
				--x;
			}
			dstrect.x = x;
		}
		k = (k + 1) % 4;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, (SDL_Texture *)texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}



	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}