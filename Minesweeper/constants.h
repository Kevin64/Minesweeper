#pragma once
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define WIDTH_MIN 2
#define WIDTH_MAX 31
#define HEIGHT_MIN 2
#define HEIGHT_MAX 16
#define MINE_MIN 0

#define ALERT "Intervalo fora do especificado!"

#define BUTTON_X 0
#define BUTTON_Y 0
#define BUTTON_W 450
#define BUTTON_H 100
#define BUTTON_SPACING 200

#define WIDTH_TEXT "Digite a largura do campo: (min 3, max 30)"
#define HEIGHT_TEXT "Digite a altura do campo: (min 3, max 15)"
#define MINE_AMOUNT_TEXT "Digite o n�mero de minas: (min 1, max alt * larg - 1)"
#define OK_BUTTON_TEXT "   OK   "

#define WIDTH_TEXT_Y 80
#define WIDTH_TEXT_W WINDOW_WIDTH / 1.75
#define WIDTH_TEXT_H 50

#define WIDTH_INPUT_Y 140
#define WIDTH_INPUT_W WINDOW_WIDTH / 30
#define WIDTH_INPUT_H 80

#define HEIGHT_TEXT_Y 240
#define HEIGHT_TEXT_W WINDOW_WIDTH / 1.75
#define HEIGHT_TEXT_H 50

#define HEIGHT_INPUT_Y 300
#define HEIGHT_INPUT_W WINDOW_WIDTH / 30
#define HEIGHT_INPUT_H 80

#define MINE_TEXT_Y 400
#define MINE_TEXT_W WINDOW_WIDTH / 1.5
#define MINE_TEXT_H 50

#define MINE_INPUT_Y 460
#define MINE_INPUT_W WINDOW_WIDTH / 30
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
#define COLOR_ALERT {0, 255, 255, 192}
#define COLOR_MENU_TEXT {0, 0, 0, 255}

#define FINISH_BANNER_X 0
#define FINISH_BANNER_Y WINDOW_HEIGHT / 3
#define FINISH_BANNER_W WINDOW_WIDTH
#define FINISH_BANNER_H 250

#define ALERT_BANNER_X 0
#define ALERT_BANNER_Y WINDOW_HEIGHT / 3
#define ALERT_BANNER_W WINDOW_WIDTH
#define ALERT_BANNER_H 120

#define X_FINE_ADJUSTEMENT 2
#define Y_FINE_ADJUSTEMENT 25
#define TEXT_BOX_FINE_ADJUSTEMENT 50

#define TITLE "Minesweeper"
#define TITLE_X 0
#define TITLE_Y 0
#define TITLE_W WINDOW_WIDTH
#define TITLE_H 50

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#define FONT_SIZE 200
#define FONT_PATH "nasalization-rg.otf"
#define ALPHA_UNSELECTED 96
#define ALPHA_SELECTED 127

#define ERROR_SDL_INIT "Error initializing SDL.\n"
#define ERROR_SDL_WINDOW "Error creating SDL Window.\n"
#define ERROR_SDL_RENDERER "Error creating SDL Renderer.\n"
#define ERROR_SDL_TTF "Error initializing SDL_TTF.\n"

#define AUTHOR "Campo Minado - v2.0.0.2109-beta2 - Implementado por Kevin Costa."

#define NEW_GAME_TEXT "Novo jogo"
#define QUIT_GAME_TEXT "Sair do jogo"

#define WIN_TEXT "Voc� venceu!"
#define LOSE_TEXT "Voc� perdeu!"

#endif