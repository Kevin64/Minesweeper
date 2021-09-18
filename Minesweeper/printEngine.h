#pragma once
#ifndef _PRINTFIELD_H_
#define _PRINTFIELD_H_
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "initField.h"

//void printField(field_t *f);

void printFinish(field_t* f, field_t* c, SDL_Renderer *renderer, SDL_Surface *finaleTextSurface, SDL_Texture *finaleTextTexture, TTF_Font *font, SDL_Color color, bool win);

void printTitle(SDL_Renderer *renderer, SDL_Surface *titleTextSurface, SDL_Texture *titleTextTexture, TTF_Font *font, SDL_Color color);

void printFormText(SDL_Renderer *renderer, SDL_Surface *formTextSurface, SDL_Texture *formTextTexture, TTF_Font *font, SDL_Color color, SDL_Rect rect, char *text);

#endif