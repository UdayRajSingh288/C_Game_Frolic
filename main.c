#include "frolic.h"

int main(void){
	SDL_Window *select_window;
	SDL_Renderer *select_renderer;
	SDL_Surface *surface;
	SDL_Texture *select_bg;
	SDL_Texture *texture_play_buttons[2];
	SDL_Texture *texture_quit_buttons[2];
	SDL_Texture *texture_dev_buttons[2];
	SDL_Point mouse_pos;
	SDL_Rect rect_play, rect_quit, rect_dev;
	SDL_bool select_loaded = SDL_FALSE, play_hltd, quit_hltd, dev_hltd;
	SDL_Event event;

	int state = SELECT;

	rect_play.w = BUTTON_WIDTH;
	rect_play.h = BUTTON_HEIGHT;
	rect_play.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	rect_play.y = SCREEN_HEIGHT / 2 - 2 * BUTTON_HEIGHT;


	rect_quit.w = BUTTON_WIDTH;
	rect_quit.h = BUTTON_HEIGHT;
	rect_quit.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	rect_quit.y = SCREEN_HEIGHT / 2;


	rect_dev.w = BUTTON_WIDTH;
	rect_dev.h = BUTTON_HEIGHT;
	rect_dev.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	rect_dev.y = SCREEN_HEIGHT / 2 + 2 * BUTTON_HEIGHT;


	SDL_Init(SDL_INIT_VIDEO);

	while (SDL_TRUE){
		switch (state){
			case SELECT:
				if (!select_loaded){
					select_window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
					select_renderer = SDL_CreateRenderer(select_window, -1, SDL_RENDERER_ACCELERATED);

					surface = SDL_LoadBMP("img/select_screen.bmp");
					select_bg = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/play_button_1.bmp");
					texture_play_buttons[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/play_button_2.bmp");
					texture_play_buttons[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/quit_button_1.bmp");
					texture_quit_buttons[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/quit_button_2.bmp");
					texture_quit_buttons[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/dev_button_1.bmp");
					texture_dev_buttons[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
					SDL_FreeSurface(surface);

					surface = SDL_LoadBMP("img/dev_button_2.bmp");
					texture_dev_buttons[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(select_renderer, surface);
					SDL_FreeSurface(surface);

					select_loaded = SDL_TRUE;
					SDL_RenderClear(select_renderer);
					SDL_RenderCopy(select_renderer, select_bg, NULL, NULL);
				}

				SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

				if (SDL_PointInRect(&mouse_pos, &rect_play)){
					play_hltd = SDL_TRUE;
					SDL_RenderCopy(select_renderer, texture_play_buttons[1], NULL, &rect_play);
				}
				else {
					play_hltd = SDL_FALSE;
					SDL_RenderCopy(select_renderer, texture_play_buttons[0], NULL, &rect_play);
				}
				if (SDL_PointInRect(&mouse_pos, &rect_quit)){
					quit_hltd = SDL_TRUE;
					SDL_RenderCopy(select_renderer, texture_quit_buttons[1], NULL, &rect_quit);
				}
				else {
					quit_hltd = SDL_FALSE;
					SDL_RenderCopy(select_renderer, texture_quit_buttons[0], NULL, &rect_quit);
				}
				if (SDL_PointInRect(&mouse_pos, &rect_dev)){
					dev_hltd = SDL_TRUE;
					SDL_RenderCopy(select_renderer, texture_dev_buttons[1], NULL, &rect_dev);
				}
				else {
					dev_hltd = SDL_FALSE;
					SDL_RenderCopy(select_renderer, texture_dev_buttons[0], NULL, &rect_dev);
				}
				SDL_RenderPresent(select_renderer);

				while (SDL_PollEvent(&event)){
					if (event.type == SDL_MOUSEBUTTONDOWN && quit_hltd){
						state = QUIT;
						select_loaded = SDL_FALSE;
						SDL_DestroyTexture(texture_dev_buttons[0]);
						SDL_DestroyTexture(texture_dev_buttons[1]);
						SDL_DestroyTexture(texture_play_buttons[0]);
						SDL_DestroyTexture(texture_play_buttons[1]);
						SDL_DestroyTexture(texture_quit_buttons[0]);
						SDL_DestroyTexture(texture_quit_buttons[1]);
						SDL_DestroyTexture(select_bg);
						SDL_DestroyRenderer(select_renderer);
						SDL_DestroyWindow(select_window);
					}
					if (event.type == SDL_MOUSEBUTTONDOWN && dev_hltd){
						state = DEV;
						select_loaded = SDL_FALSE;
						SDL_DestroyTexture(texture_dev_buttons[0]);
						SDL_DestroyTexture(texture_dev_buttons[1]);
						SDL_DestroyTexture(texture_play_buttons[0]);
						SDL_DestroyTexture(texture_play_buttons[1]);
						SDL_DestroyTexture(texture_quit_buttons[0]);
						SDL_DestroyTexture(texture_quit_buttons[1]);
						SDL_DestroyTexture(select_bg);
						SDL_DestroyRenderer(select_renderer);
						SDL_DestroyWindow(select_window);
					}
					if (event.type == SDL_MOUSEBUTTONDOWN && play_hltd){
						state = PLAY;
						select_loaded = SDL_FALSE;
						SDL_DestroyTexture(texture_dev_buttons[0]);
						SDL_DestroyTexture(texture_dev_buttons[1]);
						SDL_DestroyTexture(texture_play_buttons[0]);
						SDL_DestroyTexture(texture_play_buttons[1]);
						SDL_DestroyTexture(texture_quit_buttons[0]);
						SDL_DestroyTexture(texture_quit_buttons[1]);
						SDL_DestroyTexture(select_bg);
						SDL_DestroyRenderer(select_renderer);
						SDL_DestroyWindow(select_window);
					}
				}
				break;

			case DEV:
				show_dev_info();
				state = SELECT;
				break;

			case PLAY:
				play_game();
				state = SELECT;
				break;

			case QUIT:
				SDL_Quit();
				return 0;
		}
	}
}