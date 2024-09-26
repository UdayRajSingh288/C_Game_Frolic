#include "include/SDL2/SDL.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480
#define BUTTON_WIDTH 160
#define BUTTON_HEIGHT 40

enum {PLAY, DEV, SELECT, QUIT};

#undef main

int main(void){
	SDL_Window *window;
	SDL_Renderer *select_renderer;
	SDL_Renderer *play_renderer;
	SDL_Renderer *dev_renderer;
	SDL_Surface *surface;
	SDL_Texture *select_bg;
	SDL_Texture *play_buttons[2];
	SDL_Texture *quit_buttons[2];
	SDL_Texture *dev_buttons[2];
	SDL_Rect play_button_rect;
	SDL_Rect quit_button_rect;
	SDL_Rect dev_button_rect;
	SDL_Event event;
	SDL_bool select = SDL_TRUE;
	SDL_bool play_button_hltd = SDL_FALSE;
	SDL_bool quit_button_hltd = SDL_FALSE;
	SDL_bool dev_button_hltd = SDL_FALSE;
	SDL_bool dev_loaded = SDL_FALSE;
	SDL_Point mouse_pos;
	int state = SELECT;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("Gallops", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	select_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	play_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	dev_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	surface = SDL_LoadBMP("img/select_screen.bmp");
	select_bg = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_LoadBMP("img/play_button_1.bmp");
	play_buttons[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
	SDL_FreeSurface(surface);
	play_button_rect.w = BUTTON_WIDTH;
	play_button_rect.h = BUTTON_HEIGHT;
	play_button_rect.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	play_button_rect.y = SCREEN_HEIGHT / 2 - 2 * BUTTON_HEIGHT;

	surface = SDL_LoadBMP("img/quit_button_1.bmp");
	quit_buttons[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
	SDL_FreeSurface(surface);
	quit_button_rect.w = BUTTON_WIDTH;
	quit_button_rect.h = BUTTON_HEIGHT;
	quit_button_rect.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	quit_button_rect.y = SCREEN_HEIGHT / 2;

	surface = SDL_LoadBMP("img/dev_button_1.bmp");
	dev_buttons[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
	SDL_FreeSurface(surface);
	dev_button_rect.w = BUTTON_WIDTH;
	dev_button_rect.h = BUTTON_HEIGHT;
	dev_button_rect.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	dev_button_rect.y = SCREEN_HEIGHT / 2 + 2 * BUTTON_HEIGHT;

	surface = SDL_LoadBMP("img/play_button_2.bmp");
	play_buttons[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_LoadBMP("img/quit_button_2.bmp");
	quit_buttons[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_LoadBMP("img/dev_button_2.bmp");
	dev_buttons[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
	SDL_FreeSurface(surface);

	while (SDL_TRUE){
		switch (state){
			case DEV:
				SDL_Texture *dev_bg;
				SDL_Texture *back_button_1;
				SDL_Texture *back_button_2;
				SDL_Rect back_button;

				if (!dev_loaded){
					surface = SDL_LoadBMP("img/dev.png");
					dev_bg = (SDL_Texture *)SDL_CreateTextureFromSurface(dev_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/back_button_1.png");
					back_button_1 = (SDL_Texture *)SDL_CreateTextureFromSurface(dev_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/back_button_2.png");
					back_button_2 = (SDL_Texture *)SDL_CreateTextureFromSurface(dev_renderer, surface);
					SDL_FreeSurface(surface);

					back_button.x = 20;
					back_button.y = 20;
					back_button.w = BUTTON_WIDTH;
					back_button.h = BUTTON_HEIGHT;

					dev_loaded = SDL_TRUE;
				}

				SDL_bool back_button_hltd;

				SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

				if (SDL_PointInRect(&mouse_pos, &back_button)){
					back_button_hltd = SDL_TRUE;
				}
				else {
					back_button_hltd = SDL_FALSE;
				}

				SDL_RenderClear(dev_renderer);
				SDL_RenderCopy(dev_renderer, dev_bg, NULL, NULL);
				if (back_button_hltd){
					SDL_RenderCopy(dev_renderer, back_button_2, NULL, &back_button);
				}
				else {
					SDL_RenderCopy(dev_renderer, back_button_1, NULL, &back_button);
				}
				SDL_RenderPresent(dev_renderer);

				while (SDL_PollEvent(&event)){
					switch (event.type){
						case SDL_MOUSEBUTTONDOWN:
							if (back_button_hltd){
								state = SELECT;
								dev_loaded = SDL_FALSE;
								SDL_DestroyTexture(dev_bg);
								SDL_DestroyTexture(back_button_1);
								SDL_DestroyTexture(back_button_1);
								break;
							}
					}
				}
				break;

			case SELECT:
				SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

				if (SDL_PointInRect(&mouse_pos, &play_button_rect)){
					play_button_hltd = SDL_TRUE;
				}
				else {
					play_button_hltd = SDL_FALSE;
				}
				if (SDL_PointInRect(&mouse_pos, &quit_button_rect)){
					quit_button_hltd = SDL_TRUE;
				}
				else {
					quit_button_hltd = SDL_FALSE;
				}
				if (SDL_PointInRect(&mouse_pos, &dev_button_rect)){
					dev_button_hltd = SDL_TRUE;
				}
				else {
					dev_button_hltd = SDL_FALSE;
				}

				while (SDL_PollEvent(&event)){
					switch (event.type){
						case SDL_MOUSEBUTTONDOWN:
							if (quit_button_hltd){
								state = QUIT;
								break;
							}
							else if (play_button_hltd){
								state = PLAY;
								break;
							}
							else if (dev_button_hltd){
								state = DEV;
								break;
							}
					}
				}

				SDL_RenderClear(select_renderer);
				SDL_RenderCopy(select_renderer, select_bg, NULL, NULL);
				if (play_button_hltd){	
					SDL_RenderCopy(select_renderer, play_buttons[1], NULL, &play_button_rect);
				}
				else {
					SDL_RenderCopy(select_renderer, play_buttons[0], NULL, &play_button_rect);
				}
				if (quit_button_hltd){
					SDL_RenderCopy(select_renderer, quit_buttons[1], NULL, &quit_button_rect);
				}
				else {
					SDL_RenderCopy(select_renderer, quit_buttons[0], NULL, &quit_button_rect);
				}
				if (dev_button_hltd){
					SDL_RenderCopy(select_renderer, dev_buttons[1], NULL, &dev_button_rect);
				}
				else {
					SDL_RenderCopy(select_renderer, dev_buttons[0], NULL, &dev_button_rect);
				}
				SDL_RenderPresent(select_renderer);
				break;

			case QUIT:
				SDL_DestroyTexture(select_bg);
				SDL_DestroyTexture(play_buttons[0]);
				SDL_DestroyTexture(play_buttons[1]);
				SDL_DestroyTexture(quit_buttons[0]);
				SDL_DestroyTexture(quit_buttons[1]);
				SDL_DestroyTexture(dev_buttons[0]);
				SDL_DestroyTexture(dev_buttons[1]);
				SDL_DestroyRenderer(select_renderer);
				SDL_DestroyRenderer(dev_renderer);
				SDL_DestroyRenderer(play_renderer);
				SDL_DestroyWindow(window);
				return 0;
		}
	}
}