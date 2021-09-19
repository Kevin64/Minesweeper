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

#define WIDTH_TEXT "Digite a largura do campo:"
#define HEIGHT_TEXT "Digite a altura do campo:"
#define MINE_AMOUNT_TEXT "Digite o número de minas:"
#define OK_BUTTON_TEXT "   OK   "

#define WIDTH_TEXT_Y 60
#define WIDTH_TEXT_W WINDOW_WIDTH / 2
#define WIDTH_TEXT_H 80

#define WIDTH_INPUT_Y 140
#define WIDTH_INPUT_W WINDOW_WIDTH / 6
#define WIDTH_INPUT_H 80

#define HEIGHT_TEXT_Y 220
#define HEIGHT_TEXT_W WINDOW_WIDTH / 2
#define HEIGHT_TEXT_H 80

#define HEIGHT_INPUT_Y 300
#define HEIGHT_INPUT_W WINDOW_WIDTH / 6
#define HEIGHT_INPUT_H 80

#define MINE_TEXT_Y 380
#define MINE_TEXT_W WINDOW_WIDTH / 2
#define MINE_TEXT_H 80

#define MINE_INPUT_Y 460
#define MINE_INPUT_W WINDOW_WIDTH / 6
#define MINE_INPUT_H 80

#define OK_BUTTON_Y 600
#define OK_BUTTON_W WINDOW_WIDTH / 4
#define OK_BUTTON_H 100

#define TILE_SIDE_SIZE 35
#define TILE_SPACING 5

#define COLOR_TIP {192, 192, 192}
#define COLOR_TITLE {255, 255, 255}
#define COLOR_FORM {120, 180, 255}
#define COLOR_BUTTON {0, 0, 0}
#define COLOR_SEL_INPT {255, 255, 255}

#define FINISH_BANNER_X 0
#define FINISH_BANNER_Y WINDOW_HEIGHT / 3
#define FINISH_BANNER_W WINDOW_WIDTH
#define FINISH_BANNER_H 250

#define TITLE "Minesweeper"
#define TITLE_X 0
#define TITLE_Y 0
#define TITLE_W WINDOW_WIDTH
#define TITLE_H 50

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#define FONT_SIZE 200
#define FONT_PATH "nasalization-rg.otf"

#define ERROR_SDL_INIT "Error initializing SDL.\n"
#define ERROR_SDL_WINDOW "Error creating SDL Window.\n"
#define ERROR_SDL_RENDERER "Error creating SDL Renderer.\n"
#define ERROR_SDL_TTF "Error initializing SDL_TTF.\n"

#define AUTHOR "Campo Minado - v2.0.0.2109-beta - Implementado por Kevin Costa."

#define WIN_TEXT "Você venceu!"
#define LOSE_TEXT "Você perdeu!"

#endif