#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "printEngine.h"
#include "initField.h"
#include "fillField.h"
#include "game.h"
#include "constants.h"

//// Prints upper field to the player.
//void printField(field_t *f)
//{
//	//printTitle(); // Prints title with version.
//	for (int i = 0; i < f->x; i++)
//	{
//		printf("\n");
//		for (int j = 0; j < f->y; j++)
//		{
//			if (f->mat[i][j] == MINE)
//			{
//				printf("\033[0;31m"); // Prints in red.
//				printf(" M "); // Prints a mine.
//			}
//			else if (f->mat[i][j] == EDGE_T_B)
//			{
//				printf("\033[0;36m"); // Prints in cyan.
//				printf(" | "); // Prints left and right edges.
//			}
//			else if (f->mat[i][j] == EDGE_L_R)
//			{
//				printf("\033[0;36m"); // Prints in cyan.
//				printf(" - "); // Prints top and bottom edges.
//			}
//			else if (f->mat[i][j] == COVER)
//			{
//				printf("\033[0;37m"); // Prints in white.
//				printf(" ? "); // Prints a cover.
//			}
//			else if (f->mat[i][j] == FLAG)
//			{
//				printf("\033[0;33m"); // Prints in yellow.
//				printf(" X "); // Prints a flag.
//			}
//			else
//			{
//				printf("\033[0;32m"); // Prints in green.
//				printf(" %d ", f->mat[i][j]); // Prints a tip/zero.
//			}
//		}
//	}
//}

// Prints a victory/defeat line.
void printFinish(field_t* f, field_t* c, SDL_Renderer* renderer, SDL_Surface* finaleTextSurface, SDL_Texture* finaleTextTexture, TTF_Font* font, SDL_Color color, bool win)
{
	char* aux;
	int length;

	SDL_Rect finish_banner = {
		(int)FINISH_BANNER_X,
		(int)FINISH_BANNER_Y,
		(int)FINISH_BANNER_WIDTH,
		(int)FINISH_BANNER_HEIGHT
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
void printTitle(SDL_Renderer* renderer, SDL_Surface* titleTextSurface, SDL_Texture* titleTextTexture, TTF_Font* font, SDL_Color color)
{
	SDL_Rect title_banner = {
		(int)TITLE_X,
		(int)TITLE_Y,
		(int)TITLE_WIDTH,
		(int)TITLE_HEIGHT
	};
	int length = snprintf(NULL, 0, "%s", AUTHOR);
	char* aux = malloc(length + 1);
	snprintf(aux, length + 1, "%s", AUTHOR);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
	SDL_RenderFillRect(renderer, &title_banner);

	titleTextSurface = TTF_RenderText_Blended(font, aux, color);
	titleTextTexture = SDL_CreateTextureFromSurface(renderer, titleTextSurface);
	SDL_RenderCopy(renderer, titleTextTexture, NULL, &title_banner);
	SDL_FreeSurface(titleTextSurface);
	SDL_DestroyTexture(titleTextTexture);
}

void printFormText(SDL_Renderer* renderer, SDL_Surface* formTextSurface, SDL_Texture* formTextTexture, TTF_Font* font, SDL_Color color, SDL_Rect rect, char *text)
{
	int length = snprintf(NULL, 0, "%s", text);
	char* aux = malloc(length + 1);
	snprintf(aux, length + 1, "%s", text);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
	SDL_RenderFillRect(renderer, &rect);

	formTextSurface = TTF_RenderText_Blended(font, aux, color);
	formTextTexture = SDL_CreateTextureFromSurface(renderer, formTextSurface);
	SDL_RenderCopy(renderer, formTextTexture, NULL, &rect);
	SDL_FreeSurface(formTextSurface);
	SDL_DestroyTexture(formTextTexture);
}