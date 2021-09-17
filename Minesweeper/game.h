#pragma once
#ifndef _GAME_H_
#define _GAME_H_
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "initField.h"

bool checkWin(field_t *f, field_t *c);

bool checkLose(field_t *f, field_t *c, int *inpt);

void printFinish(field_t *f, field_t *c, SDL_Renderer *renderer, SDL_Surface *finaleTextSurface, SDL_Texture *finaleTextTexture, TTF_Font *font, SDL_Color color, bool win);

void printTitle(SDL_Renderer *renderer, SDL_Surface *titleTextSurface, SDL_Texture *titleTextTexture, TTF_Font *font, SDL_Color color);

int *verifyInput(field_t *f);

void startGame();

#endif