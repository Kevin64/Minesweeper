#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "initField.h"
#include "fillField.h"
#include "printField.h"
#include "game.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Color color;
TTF_Font* font;
SDL_Surface* textSurface;
SDL_Texture* textTexture;

bool game_is_running = true;
int last_frame_time = 0;
int length;
float delta_time = 0.0f;
char* aux;
field_t *f, *c;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (!window)
	{
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return false;
	}

	if (TTF_Init() != 0)
	{
		fprintf(stderr, "Error initializing SDL_TTF.\n");
		return false;
	}
	font = TTF_OpenFont("C:\\tahoma.ttf" /*path*/, 48 /*size*/);

	return true;
}

void setup(int w, int h, int m)
{
	tile.x = 10;
	tile.y = 10;
	tile.w = TILE_SIDE_SIZE;
	tile.h = TILE_SIDE_SIZE;

	f = initField(w, h, m); // Allocates the lower field, where mines and tips will be written.
	c = initCover(w, h); // Allocates the upper field, where the player will uncover.
	fillFieldEdge(c); // Fills the upper field with edge characters.
	fillFieldCover(c); // Fills the upper field with cover characters.
	fillFieldZero(f); // Fills the lower field with zeros.
	fillFieldEdge(f); // Fills the upper field with edge characters.
	fillFieldMine(f); // Fills the lower field with mines
	countMines(f); // Calculates the amount of mines and fills tips on mines's neighborhoods.
}

void process_input()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		game_is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			game_is_running = false;
		break;
	}
}

void update()
{
	// Sleep the execution until we reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

	// Only call delay if we are too fast to process this frame
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);

	// Get a delta time factor converted to seconds to be used to update my objects
	delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();

	//TODO: Here is where we can update the state of our objects

	
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//TODO: Here is where we can start drawing ou game objects

	for (int i = 0; i < f->x; i++)
	{
		for (int j = 0; j < f->y; j++)
		{
			SDL_Rect tile_square = {
			(int)(tile.x + tile.w) * j,
			(int)(tile.y + tile.h) * i,
			(int)tile.w,
			(int)tile.h
			};
			if (f->mat[i][j] == MINE)
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &tile_square);
			}
			else if (f->mat[i][j] == EDGE_L_R || f->mat[i][j] == EDGE_T_B)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
				SDL_RenderFillRect(renderer, &tile_square);
			}
			else
			{
				length = snprintf(NULL, 0, "%d", f->mat[i][j]);
				aux = malloc(length + 1);
				snprintf(aux, length + 1, "%d", f->mat[i][j]);

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &tile_square);

				textSurface = TTF_RenderText_Blended(font, aux, color);
				textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
				SDL_RenderCopy(renderer, textTexture, NULL, &tile_square);
				SDL_FreeSurface(textSurface);
				SDL_DestroyTexture(textTexture);
			}			
		}		
	}
	SDL_RenderPresent(renderer);
}

void destroy_window()
{
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

int main()
{
	//startGame(); //Starts a new game.
	//while (1)
	//{
	//	// Checks if the player wants to play again. If not, exit the program.
	//	//char c = ' ';
	//	//printf("\033[0;37m"); // Prints in white.
	//	//printf("\n Deseja jogar novamente? (S ou s para confirmar, qualquer outra tecla para sair)");
	//	//scanf_s(" %c", &c, sizeof(char));
	//	if (c == 'S' || c == 's')
	//	{
	//		//system("cls"); // Clear the screen.
	//		startGame(); // Starts a new game.
	//	}
	//	else
	//		exit(0); // Closes game.
	//}

	game_is_running = initialize_window();

	setup(10, 10, 10);
	printField(f);
	while (game_is_running)
	{
		process_input();
		update();
		render();
	}
	destroy_window();
	return 0;
}