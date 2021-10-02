#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "printEngine.h"
#include "initField.h"
#include "fillField.h"
#include "game.h"
#include "constants.h"

// Prints an alert line.
void printAlert(SDL_Renderer * renderer, SDL_Surface * alertTextSurface, SDL_Texture * alertTextTexture, TTF_Font * font, SDL_Color color)
{
	char *aux;
	int length;

	SDL_Rect alert_banner = {
		(int)ALERT_BANNER_X,
		(int)ALERT_BANNER_Y,
		(int)ALERT_BANNER_W,
		(int)ALERT_BANNER_H
	};
	
	length = snprintf(NULL, 0, "%s", ALERT);
	aux = malloc(length + 1);
	snprintf(aux, length + 1, "%s", ALERT);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);

	SDL_RenderFillRect(renderer, &alert_banner);

	alertTextSurface = TTF_RenderText_Blended(font, aux, color);
	alertTextTexture = SDL_CreateTextureFromSurface(renderer, alertTextSurface);
	SDL_RenderCopy(renderer, alertTextTexture, NULL, &alert_banner);
	SDL_FreeSurface(alertTextSurface);
	SDL_DestroyTexture(alertTextTexture);
}

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
void printTitle(SDL_Renderer *renderer, SDL_Surface *aboutTextSurface, SDL_Texture *aboutTextTexture, TTF_Font *font, SDL_Color color)
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

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &title_banner);

	aboutTextSurface = TTF_RenderText_Blended(font, aux, color);
	aboutTextTexture = SDL_CreateTextureFromSurface(renderer, aboutTextSurface);
	SDL_RenderCopy(renderer, aboutTextTexture, NULL, &title_banner);
	SDL_FreeSurface(aboutTextSurface);
	SDL_DestroyTexture(aboutTextTexture);
}

void printTextLine(SDL_Renderer *renderer, SDL_Surface *formTextSurface, SDL_Texture *formTextTexture, TTF_Font *font, SDL_Color color, SDL_Rect rect, char *text, int r, int g, int b, int a)
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