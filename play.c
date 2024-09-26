#include "frolic.h"
#include <time.h>
#include <stdlib.h>

#define BG_SPEED 4
#define PLAYER_ANIM_LEN 2
#define PLAYER_FALL_SPEED 2
#define PLAYER_ANIM_DELAY 250
#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 84

#define SNAIL_WIDTH 72
#define SNAIL_HEIGHT 36
#define FLY_WIDTH 84
#define FLY_HEIGHT 40
#define SNAIL_SPEED 2
#define FLY_SPEED 2

static int absolute(int n){
	if (n < 0){
		return -n;
	}
	return n;
}

void play_game(void){
	SDL_Window *play_window;
	SDL_Renderer *play_renderer;
	SDL_Surface *surface;
	SDL_Texture *tex_bg;
	SDL_Texture *tex_back[2];
	SDL_Texture *tex_resume[2];
	SDL_Texture *tex_player[3];
	SDL_Texture *current_tex_player;
	SDL_Texture *tex_snail[2];
	SDL_Texture *current_tex_snail;
	SDL_Texture *tex_fly[2];
	SDL_Texture *current_tex_fly;
	SDL_Rect rect_bg_1, rect_bg_2, rect_back, rect_resume, rect_player, rect_snail, rect_fly;
	SDL_Event event;
	SDL_bool play_loaded = SDL_FALSE, update_loop = SDL_TRUE, other_loop, back_hltd, resume_hltd, quit = SDL_FALSE, go_down = SDL_FALSE, game_over = SDL_FALSE;
	SDL_Point mouse_pos;
	int bg_time_last, bg_time_now, snail_time_last, fly_time_last, player_time_last;
	int player_anim_ind = 0, player_time_now;
	int time_now, time_last, snail_time_now, fly_time_now;

	rect_bg_1.x = 0;
	rect_bg_1.y = 0;
	rect_bg_1.w = SCREEN_WIDTH;
	rect_bg_1.h = SCREEN_HEIGHT;

	rect_bg_2.x = SCREEN_WIDTH;
	rect_bg_2.y = 0;
	rect_bg_2.w = SCREEN_WIDTH;
	rect_bg_2.h = SCREEN_HEIGHT;

	rect_back.w = BUTTON_WIDTH;
	rect_back.h = BUTTON_HEIGHT;
	rect_back.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	rect_back.y = SCREEN_HEIGHT / 2 - 2 * BUTTON_HEIGHT;

	rect_resume.w = BUTTON_WIDTH;
	rect_resume.h = BUTTON_HEIGHT;
	rect_resume.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
	rect_resume.y = SCREEN_HEIGHT / 2 + 2 * BUTTON_HEIGHT;

	rect_player.w =  PLAYER_WIDTH;
	rect_player.h =  PLAYER_HEIGHT;
	rect_player.x =  PLAYER_WIDTH;
	rect_player.y = SCREEN_HEIGHT - 3 * PLAYER_HEIGHT / 2;

	rect_snail.y = SCREEN_HEIGHT - PLAYER_HEIGHT;
	rect_snail.w = SNAIL_WIDTH;
	rect_snail.h = SNAIL_HEIGHT;

	rect_fly.y = SCREEN_HEIGHT - 2 * PLAYER_HEIGHT - 5;
	rect_fly.w = FLY_WIDTH;
	rect_fly.h = FLY_HEIGHT;

	while (quit == SDL_FALSE){
		if (play_loaded == SDL_FALSE){
			srand(time(NULL));

			play_window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);

			play_renderer = SDL_CreateRenderer(play_window, -1, SDL_RENDERER_ACCELERATED);

			surface = SDL_LoadBMP("img/bg.bmp");
			tex_bg = SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			surface = SDL_LoadBMP("img/player_walk_1.bmp");
			tex_player[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			surface = SDL_LoadBMP("img/player_walk_2.bmp");
			tex_player[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			surface = SDL_LoadBMP("img/player_jump.bmp");
			tex_player[2] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			surface = SDL_LoadBMP("img/snail1.bmp");
			tex_snail[0] = SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			surface = SDL_LoadBMP("img/snail2.bmp");
			tex_snail[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			surface = SDL_LoadBMP("img/fly1.bmp");
			tex_fly[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			surface = SDL_LoadBMP("img/fly2.bmp");
			tex_fly[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
			SDL_FreeSurface(surface);

			time_last = SDL_GetTicks();
			bg_time_last = SDL_GetTicks();
			snail_time_last = SDL_GetTicks();
			fly_time_last = SDL_GetTicks();

			rect_snail.x = rand() % SCREEN_WIDTH;
			rect_snail.x += SCREEN_WIDTH;
			rect_fly.x = rand() % SCREEN_WIDTH;
			rect_fly.x += SCREEN_WIDTH;

	
			if (absolute(rect_fly.x - rect_snail.x) < 3 * PLAYER_WIDTH){
				if (rect_fly.x < rect_snail.x){
					rect_snail.x += 3 * PLAYER_WIDTH;
				}
				else {
					rect_fly.x += 3 * PLAYER_WIDTH;
				}
			}

			current_tex_player = tex_player[0];
			current_tex_snail = tex_snail[0];
			current_tex_fly = tex_fly[0];

			play_loaded = SDL_TRUE;
		}

		while (update_loop){
			bg_time_now = SDL_GetTicks();
			if (bg_time_now - bg_time_last >= 5){
				bg_time_last = bg_time_now;
				if (rect_bg_1.x < -SCREEN_WIDTH){
					rect_bg_1.x = SCREEN_WIDTH;
				}
				else {
					--rect_bg_1.x;
				}
				if (rect_bg_2.x < -SCREEN_WIDTH){
					rect_bg_2.x = SCREEN_WIDTH;
				}
				else {
					--rect_bg_2.x;
				}
			}
			if (rect_player.y == SCREEN_HEIGHT - 3 * PLAYER_HEIGHT / 2){
				go_down = SDL_FALSE;
				time_now = SDL_GetTicks();
				if (time_now - time_last >= PLAYER_ANIM_DELAY){
					time_last = time_now;
					player_anim_ind = (player_anim_ind + 1) % PLAYER_ANIM_LEN;
					current_tex_player = tex_player[player_anim_ind];
					current_tex_snail = tex_snail[player_anim_ind];
					current_tex_fly = tex_fly[player_anim_ind];
				}
			}
			else if (go_down) {
				player_anim_ind = 0;
				player_time_now = SDL_GetTicks();
				if (player_time_now - player_time_last >= 2){
					player_time_last = player_time_now;
					rect_player.y += 1;
				}
				current_tex_player = tex_player[2];
			}
			else {
				player_anim_ind = 0;
				player_time_now = SDL_GetTicks();
				if (player_time_now - player_time_last >= 2){
					player_time_last = player_time_now;
					--rect_player.y;
				}
				current_tex_player = tex_player[2];
				if (rect_player.y == SCREEN_HEIGHT - 3 * PLAYER_HEIGHT){
					go_down = SDL_TRUE;
				}
			}

			if (rect_snail.x <= -SNAIL_WIDTH){
				rect_snail.x = rand() % SCREEN_WIDTH;
				rect_snail.x += SCREEN_WIDTH;
				if (absolute(rect_fly.x - rect_snail.x) < 3 * PLAYER_WIDTH){
					if (rect_fly.x < rect_snail.x){
						rect_snail.x += 3 * PLAYER_WIDTH;
					}
					else {
						rect_fly.x += 3 * PLAYER_WIDTH;
					}
				}
			}
			else {
				snail_time_now = SDL_GetTicks();
				if (snail_time_now - snail_time_last >= 2){
					snail_time_last = snail_time_now;
					--rect_snail.x;
				}
			}
			if (rect_fly.x <= -FLY_WIDTH){
				rect_fly.x = rand() % SCREEN_WIDTH;
				rect_fly.x += SCREEN_WIDTH;
				if (absolute(rect_fly.x - rect_snail.x) < 3 * PLAYER_WIDTH){
					if (rect_fly.x < rect_snail.x){
						rect_snail.x += 3 * PLAYER_WIDTH;
					}
					else {
						rect_fly.x += 3 * PLAYER_WIDTH;
					}
				}
			}
			else {
				fly_time_now = SDL_GetTicks();
				if (fly_time_now - fly_time_last >= 2){
					fly_time_last = fly_time_now;
					--rect_fly.x;
				}
			}

			if (SDL_HasIntersection(&rect_player, &rect_snail) || SDL_HasIntersection(&rect_player, &rect_fly)){
					update_loop = SDL_FALSE;
					other_loop = SDL_TRUE;
					game_over = SDL_TRUE;
					break;
			}

			SDL_RenderCopy(play_renderer, tex_bg, NULL, &rect_bg_1);
			SDL_RenderCopy(play_renderer, tex_bg, NULL, &rect_bg_2);
			SDL_RenderCopy(play_renderer, current_tex_player, NULL, &rect_player);
			SDL_RenderCopy(play_renderer, current_tex_snail, NULL, &rect_snail);
			SDL_RenderCopy(play_renderer, current_tex_fly, NULL, &rect_fly);
			SDL_RenderPresent(play_renderer);

			while (SDL_PollEvent(&event)){
				if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
					update_loop = SDL_FALSE;
					other_loop = SDL_TRUE;
					break;
				}
				if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE && rect_player.y == SCREEN_HEIGHT - 3 * PLAYER_HEIGHT / 2){
					--rect_player.y;
					player_time_last = SDL_GetTicks();
				}
			}
		}

		surface = SDL_LoadBMP("img/back_button_1.bmp");
		tex_back[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
		SDL_FreeSurface(surface);

		surface = SDL_LoadBMP("img/back_button_2.bmp");
		tex_back[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
		SDL_FreeSurface(surface);

		surface = SDL_LoadBMP("img/resume_button_1.bmp");
		tex_resume[0] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
		SDL_FreeSurface(surface);

		surface = SDL_LoadBMP("img/resume_button_2.bmp");
		tex_resume[1] = (SDL_Texture *)SDL_CreateTextureFromSurface(play_renderer, surface);
		SDL_FreeSurface(surface);

		while (other_loop){
			SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

			if (SDL_PointInRect(&mouse_pos, &rect_back)){
				back_hltd = SDL_TRUE;
				SDL_RenderCopy(play_renderer, tex_back[1], NULL, &rect_back);
			}
			else {
				back_hltd = SDL_FALSE;
				SDL_RenderCopy(play_renderer, tex_back[0], NULL, &rect_back);
			}

			if (SDL_PointInRect(&mouse_pos, &rect_resume)){
				resume_hltd = SDL_TRUE;
				SDL_RenderCopy(play_renderer, tex_resume[1], NULL, &rect_resume);
			}
			else {
				resume_hltd = SDL_FALSE;
				SDL_RenderCopy(play_renderer, tex_resume[0], NULL, &rect_resume);
			}

			SDL_RenderPresent(play_renderer);

			while (SDL_PollEvent(&event)){
				if (event.type == SDL_MOUSEBUTTONDOWN && resume_hltd){
					other_loop = SDL_FALSE;
					update_loop =SDL_TRUE;
					if (game_over){
						game_over = SDL_FALSE;
						play_loaded = SDL_FALSE;
						SDL_DestroyTexture(tex_snail[0]);
						SDL_DestroyTexture(tex_snail[1]);
						SDL_DestroyTexture(tex_fly[0]);
						SDL_DestroyTexture(tex_fly[1]);
						SDL_DestroyTexture(tex_player[0]);
						SDL_DestroyTexture(tex_player[1]);
						SDL_DestroyTexture(tex_player[2]);
						SDL_DestroyTexture(tex_bg);
						SDL_DestroyRenderer(play_renderer);
						SDL_DestroyWindow(play_window);
					}
					break;
				}
				if (event.type == SDL_MOUSEBUTTONDOWN && back_hltd){
					other_loop = SDL_FALSE;
					quit = SDL_TRUE;
					break;
				}
			}
		}

		SDL_DestroyTexture(tex_back[0]);
		SDL_DestroyTexture(tex_back[1]);
		SDL_DestroyTexture(tex_resume[0]);
		SDL_DestroyTexture(tex_resume[1]);
	}
	SDL_DestroyTexture(tex_snail[0]);
	SDL_DestroyTexture(tex_snail[1]);
	SDL_DestroyTexture(tex_fly[0]);
	SDL_DestroyTexture(tex_fly[1]);
	SDL_DestroyTexture(tex_player[0]);
	SDL_DestroyTexture(tex_player[1]);
	SDL_DestroyTexture(tex_player[2]);
	SDL_DestroyTexture(tex_bg);
	SDL_DestroyRenderer(play_renderer);
	SDL_DestroyWindow(play_window);
}