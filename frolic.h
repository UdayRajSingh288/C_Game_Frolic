#include "include/SDL2/SDL.h"

#undef main

enum {SELECT, DEV, PLAY, QUIT};

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 480
#define BUTTON_WIDTH 160
#define BUTTON_HEIGHT 40

void show_dev_info(void);
void play_game(void);