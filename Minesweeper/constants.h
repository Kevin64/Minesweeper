#pragma once
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define BUTTON_X 0
#define BUTTON_Y 0
#define BUTTON_W 250
#define BUTTON_H 100
#define BUTTON_SPACING 200

#define TILE_SIDE_SIZE 35
#define TILE_SPACING 5

#define COLOR_TIP {192, 192, 192}
#define COLOR_TITLE {255, 255, 255}
#define COLOR_SEL_INPT {255, 255, 255}

#define FINISH_BANNER_X 0
#define FINISH_BANNER_Y WINDOW_HEIGHT / 3
#define FINISH_BANNER_WIDTH WINDOW_WIDTH
#define FINISH_BANNER_HEIGHT 250

#define TITLE "Minesweeper"
#define TITLE_X 0
#define TITLE_Y 0
#define TITLE_WIDTH WINDOW_WIDTH
#define TITLE_HEIGHT 50

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#define FONT_SIZE 200
#define FONT_PATH "nasalization-rg.otf"

#define ERROR_SDL_INIT "Error initializing SDL.\n"
#define ERROR_SDL_WINDOW "Error creating SDL Window.\n"
#define ERROR_SDL_RENDERER "Error creating SDL Renderer.\n"
#define ERROR_SDL_TTF "Error initializing SDL_TTF.\n"

#define AUTHOR "Campo Minado - v2.0.0.2109-alpha - Implementado por Kevin Costa."

#define WIN_TEXT "Você venceu!"
#define LOSE_TEXT "Você perdeu!"

#define WIDTH_TEXT "Digite a largura do campo:"
#define HEIGHT_TEXT "Digite a altura do campo:"
#define MINE_AMOUNT_TEXT "Digite o número de minas:"
#define OK_BUTTON "OK"

#endif