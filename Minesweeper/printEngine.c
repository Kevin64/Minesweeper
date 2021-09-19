#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "printEngine.h"
#include "initField.h"
#include "fillField.h"
#include "game.h"
#include "constants.h"

// Prints a victory/defeat line.
void printFinish(field_t *f, field_t *c, SDL_Renderer *renderer, SDL_Surface *finaleTextSurface, SDL_Texture *finaleTextTexture, TTF_Font *font, SDL_Color color, bool win)
{
	char *aux;
	int length;

	SDL_Rect finish_banner = {
		(int)FINISH_BANNER_X,
		(int)FINISH_BANNER_Y,
		(int)FINISH_BANNER_W,
		(int)FINISH_BANNER_H
	};

	if (win)
	{
		length = snprintf(NULL, 0, "%s", WIN_TEXT);
		aux = malloc(length + 1);
		snprintf(aux, length + 1, "%s", WIN_TEXT);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 127);
	}
	else
	{
		length = snprintf(NULL, 0, "%s", LOSE_TEXT);
		aux = malloc(length + 1);
		snprintf(aux, length + 1, "%s", LOSE_TEXT);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
	}

	SDL_RenderFillRect(renderer, &finish_banner);

	finaleTextSurface = TTF_RenderText_Blended(font, aux, color);
	finaleTextTexture = SDL_CreateTextureFromSurface(renderer, finaleTextSurface);
	SDL_RenderCopy(renderer, finaleTextTexture, NULL, &finish_banner);
	SDL_FreeSurface(finaleTextSurface);
	SDL_DestroyTexture(finaleTextTexture);

}

// Prints title with version.
void printTitle(SDL_Renderer *renderer, SDL_Surface *titleTextSurface, SDL_Texture *titleTextTexture, TTF_Font *font, SDL_Color color)
{
	SDL_Rect title_banner = {
		(int)TITLE_X,
		(int)TITLE_Y,
		(int)TITLE_W,
		(int)TITLE_H
	};
	int length = snprintf(NULL, 0, "%s", AUTHOR);
	char *aux = malloc(length + 1);
	snprintf(aux, length + 1, "%s", AUTHOR);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
	SDL_RenderFillRect(renderer, &title_banner);

	titleTextSurface = TTF_RenderText_Blended(font, aux, color);
	titleTextTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);
	SDL_RenderCopy(renderer, titleTextTexture, NULL, &title_banner);
	SDL_FreeSurface(titleTextSurface);
	SDL_DestroyTexture(titleTextTexture);
}

void printFormText(SDL_Renderer *renderer, SDL_Surface *formTextSurface, SDL_Texture *formTextTexture, TTF_Font *font, SDL_Color color, SDL_Rect rect, char *text, int r, int g, int b, int a)
{
	int length = snprintf(NULL, 0, "%s", text);
	char *aux = malloc(length + 1);
	snprintf(aux, length + 1, "%s", text);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);

	formTextSurface = TTF_RenderText_Blended(font, aux, color);
	formTextTexture = SDL_CreateTextureFromSurface(renderer, formTextSurface);
	SDL_RenderCopy(renderer, formTextTexture, NULL, &rect);
	SDL_FreeSurface(formTextSurface);
	SDL_DestroyTexture(formTextTexture);
}