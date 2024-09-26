#include "frolic.h"


void show_dev_info(void){
	SDL_Window *dev_window;
	SDL_Renderer *dev_renderer;
	SDL_Surface *surface;
	SDL_Texture *dev_bg, *back_tex_1, *back_tex_2;
	SDL_Rect rect;
	SDL_Point mouse_pos;
	SDL_Event event;
	SDL_bool loop = SDL_TRUE, back_hltd;

	rect.x = 20;
	rect.y = 20;
	rect.w = BUTTON_WIDTH;
	rect.h = BUTTON_HEIGHT;

	dev_window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
	dev_renderer = SDL_CreateRenderer(dev_window, -1, SDL_RENDERER_ACCELERATED);

	surface = SDL_LoadBMP("img/dev.bmp");
	dev_bg = (SDL_Texture *)SDL_CreateTextureFromSurface(dev_renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_LoadBMP("img/back_button_1.bmp");
	back_tex_1 = (SDL_Texture *)SDL_CreateTextureFromSurface(dev_renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_LoadBMP("img/back_button_2.bmp");
	back_tex_2 = (SDL_Texture *)SDL_CreateTextureFromSurface(dev_renderer, surface);
	SDL_FreeSurface(surface);

	SDL_RenderClear(dev_renderer);
	SDL_RenderCopy(dev_renderer, dev_bg, NULL, NULL);

	while (loop){
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

		if (SDL_PointInRect(&mouse_pos, &rect)){
			back_hltd = SDL_TRUE;
			SDL_RenderCopy(dev_renderer, back_tex_2, NULL, &rect);
		}
		else {
			back_hltd = SDL_FALSE;
			SDL_RenderCopy(dev_renderer, back_tex_1, NULL, &rect);
		}

		SDL_RenderPresent(dev_renderer);

		while (SDL_PollEvent(&event)){
			if (event.type == SDL_MOUSEBUTTONDOWN && back_hltd){
				loop = SDL_FALSE;
				SDL_DestroyTexture(dev_bg);
				SDL_DestroyTexture(back_tex_1);
				SDL_DestroyTexture(back_tex_2);
				SDL_DestroyRenderer(dev_renderer);
				SDL_DestroyWindow(dev_window);
				break;
			}
		}
	}
}