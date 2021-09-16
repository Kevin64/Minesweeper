#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "initField.h"
#include "fillField.h"
#include "printField.h"
#include "openSpaceField.h"
#include "game.h"
#include "constants.h"

// SDL global variables.
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;
SDL_Color color = {192, 192, 192};
TTF_Font *font;
SDL_Surface *menuTextSurface1, *menuTextSurface2, *finaleTextSurface, *gameTextSurface;
SDL_Texture *menuTextTexture1, *menuTextTexture2, *finaleTextTexture, *gameTextTexture;
SDL_Rect new_game_button, quit_game_button, tile_square;

// Game global variables.
bool game_is_running = true;
bool stage_is_running = false;
bool menu_is_running = false;
bool clickedL = false;
bool clickedR = false;
bool win = false;
bool lose = false;
bool canOpen = false;
bool canFlag = false;
int last_frame_time = 0;
int length, option = 0;
int i, j;
int ij_selected[3];
int xm, ym, xi, xf, yi, yf;
int button_x, button_y, button_w, button_h;
float delta_time = 0.0f;
char* aux;
field_t *f, *c;

// Initializes window.
bool initialize_window(void)
{
	// Initializes SDL, if can't, outputs an error message.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, ERROR_SDL_INIT);
		return false;
	}
	// Create window with those parameters.
	window = SDL_CreateWindow(
		TITLE, // Title in titlebar.
		SDL_WINDOWPOS_CENTERED, // Screen X position.
		SDL_WINDOWPOS_CENTERED, // Screen Y position.
		WINDOW_WIDTH, // Window width.
		WINDOW_HEIGHT, // Window height.
		SDL_WINDOW_ALLOW_HIGHDPI // Flag for HiDPI support.
	);
	// If can't create window, outputs an error message.
	if (!window)
	{
		fprintf(stderr, ERROR_SDL_WINDOW);
		return false;
	}
	// Initializes SDL renderer.
	renderer = SDL_CreateRenderer(window, -1, 0);
	// If can't create renderer, outputs an error message.
	if (!renderer)
	{
		fprintf(stderr, ERROR_SDL_RENDERER);
		return false;
	}

	// Initializes the font through SDL_TTF.
	if (TTF_Init() != 0)
	{
		fprintf(stderr, ERROR_SDL_TTF);
		return false;
	}
	font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
	
	// Sets the alpha channel for blending.
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	// Returns true if all is ok.
	return true;
}

// Initializes menu parameters.
void setup_menu()
{
	// Sets button size.
	button_x = DELTA_X;
	button_y = WINDOW_HEIGHT;
	button_w = WINDOW_WIDTH - 2*DELTA_X;
	button_h = DELTA_Y;

	// New game button position.
	new_game_button.x = button_x;
	new_game_button.y = button_y / 4;
	new_game_button.w = button_w;
	new_game_button.h = button_h;
	
	// Quit game button position.
	quit_game_button.x = button_x;
	quit_game_button.y = new_game_button.y * 3 - button_h;
	quit_game_button.w = button_w;
	quit_game_button.h = button_h;
}

// Initializes stage parameters.
void setup_stage(int w, int h, int m)
{
	tile.x = TILE_SPACING; // Tile spacing between each other.
	tile.y = TILE_SPACING; // Tile spacing between each other.
	tile.w = TILE_SIDE_SIZE; // Tile side size.
	tile.h = TILE_SIDE_SIZE; // Tile side size.

	f = initField(w, h, m); // Allocates the lower field, where mines and tips will be written.
	c = initCover(w, h); // Allocates the upper field, where the player will uncover.
	fillFieldEdge(c); // Fills the upper field with edge characters.
	fillFieldCover(c); // Fills the upper field with cover characters.
	fillFieldZero(f); // Fills the lower field with zeros.
	fillFieldEdge(f); // Fills the upper field with edge characters.
	fillFieldMine(f); // Fills the lower field with mines
	countMines(f); // Calculates the amount of mines and fills tips on mines's neighborhoods.
}

// Process user mouse/keyboard inputs in menu.
void process_input()
{
	while (SDL_PollEvent(&event))
	{
		if (menu_is_running && !stage_is_running)
		{			
			switch (event.type)
			{
			case SDL_QUIT:
				game_is_running = false;
				menu_is_running = false;
				stage_is_running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					game_is_running = false;
					menu_is_running = false;
					stage_is_running = false;
				}
				if (event.key.keysym.sym == SDLK_DOWN && option < 1)
					option++;
				if (event.key.keysym.sym == SDLK_UP && option > 0)
					option--;
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					if (option == 0)
					{
						menu_is_running = false;
						stage_is_running = true;
					}
					else if (option == 1)
					{
						game_is_running = false;
						menu_is_running = false;
						stage_is_running = false;
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				clickedL = event.button.button == SDL_BUTTON_LEFT;
				break;
			case SDL_MOUSEBUTTONUP:
				clickedL = !event.button.button == SDL_BUTTON_LEFT;
				break;
			}			
		}
		else if (!menu_is_running && stage_is_running)
		{			
			switch (event.type)
			{
			case SDL_QUIT:
				game_is_running = false;
				menu_is_running = false;
				stage_is_running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					game_is_running = false;
					menu_is_running = false;
					stage_is_running = false;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				clickedL = event.button.button == SDL_BUTTON_LEFT;
				clickedR = event.button.button == SDL_BUTTON_RIGHT;
				break;
			case SDL_MOUSEBUTTONUP:
				clickedL = !event.button.button == SDL_BUTTON_LEFT;
				clickedR = !event.button.button == SDL_BUTTON_RIGHT;
				break;
			}			
		}
	}
	SDL_GetMouseState(&xm, &ym);	
}

// Process game objects states in game.
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
	
	if (f != NULL && c != NULL)
	{
		if (canOpen)
			openField(f, c, ij_selected[0], ij_selected[1], ij_selected[2]);
		if (canFlag)
			openField(f, c, ij_selected[0], ij_selected[1], ij_selected[2]);
		win = checkWin(f, c);
		lose = checkLose(f, c, ij_selected);
		canOpen = canFlag = false;
	}
}

// Process object rendering in game.
void render()
{
	if (!stage_is_running && menu_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (xm >= new_game_button.x && xm <= new_game_button.x + new_game_button.w && ym >= new_game_button.y && ym <= new_game_button.y + new_game_button.h)
		{
			option = 0;
			if (clickedL)
			{
				menu_is_running = false;
				stage_is_running = true;
				clickedL = false;
			}
		}
		if (xm >= quit_game_button.x && xm <= quit_game_button.x + quit_game_button.w && ym >= quit_game_button.y && ym <= quit_game_button.y + quit_game_button.h)
		{
			option = 1;
			if (clickedL)
			{
				game_is_running = false;
				menu_is_running = false;
				stage_is_running = false;
				clickedL = false;
			}
		}
		if (option == 0)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &new_game_button);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &quit_game_button);
		}
		else if (option == 1)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &quit_game_button);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &new_game_button);
		}

		SDL_RenderPresent(renderer);
	}
	else if (stage_is_running && !menu_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		i = 0;
		j = 0;
		float centerFieldX = ((WINDOW_WIDTH / 2) - (c->x * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);
		float centerFieldY = ((WINDOW_HEIGHT / 2) - (c->y * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);

		//TODO: Here is where we can start drawing ou game objects

		for (i = 0; i < c->x; i++)
		{
			for (j = 0; j < c->y; j++)
			{
				xi = (tile.x + tile.w) * i + centerFieldX;
				yi = (tile.y + tile.h) * j + centerFieldY;
				xf = (xi + tile.w);
				yf = (yi + tile.h);

				tile_square.x = xi;
				tile_square.y = yi;
				tile_square.w = tile.w;
				tile_square.h = tile.h;

				if (xm >= xi && xm <= xf && ym >= yi && ym <= yf && c->mat[i][j] != EDGE_L_R && c->mat[i][j] != EDGE_T_B)
				{
					if (clickedL)
					{
						canOpen = true;
						ij_selected[0] = i;
						ij_selected[1] = j;
						ij_selected[2] = OPEN_F;
					}
					if (clickedR)
					{
						canFlag = true;
						ij_selected[0] = i;
						ij_selected[1] = j;
						ij_selected[2] = FLAG_F;
					}
					clickedL = false;
					clickedR = false;
				}
				if (c->mat[i][j] == MINE)
				{
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					SDL_RenderFillRect(renderer, &tile_square);
				}
				else if (c->mat[i][j] == EDGE_L_R || c->mat[i][j] == EDGE_T_B)
				{
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
					SDL_RenderFillRect(renderer, &tile_square);
				}
				else if (c->mat[i][j] == FLAG)
				{
					SDL_SetRenderDrawColor(renderer, 127, 255, 0, 255);
					SDL_RenderFillRect(renderer, &tile_square);
				}
				else if (c->mat[i][j] == COVER)
				{
					SDL_SetRenderDrawColor(renderer, 64, 64, 128, 255);
					SDL_RenderFillRect(renderer, &tile_square);
				}
				else if (c->mat[i][j] == 0)
				{
					SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
					SDL_RenderFillRect(renderer, &tile_square);
				}
				else
				{
					length = snprintf(NULL, 0, "%d", c->mat[i][j]);
					aux = malloc(length + 1);
					snprintf(aux, length + 1, "%d", c->mat[i][j]);

					SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);
					SDL_RenderFillRect(renderer, &tile_square);

					gameTextSurface = TTF_RenderText_Blended(font, aux, color);
					gameTextTexture = SDL_CreateTextureFromSurface(renderer, gameTextSurface);
					SDL_RenderCopy(renderer, gameTextTexture, NULL, &tile_square);
					SDL_FreeSurface(gameTextSurface);
					SDL_DestroyTexture(gameTextTexture);
				}
			}
		}
		if (win)
		{
			SDL_Rect win_banner = {
				(int)0,
				(int)WINDOW_HEIGHT / 3,
				(int)WINDOW_WIDTH,
				(int)100
			};
			length = snprintf(NULL, 0, "%s", "Você venceu!");
			aux = malloc(length + 1);
			snprintf(aux, length + 1, "%s", "Você venceu!");

			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 127);
			SDL_RenderFillRect(renderer, &win_banner);

			finaleTextSurface = TTF_RenderText_Blended(font, aux, color);
			finaleTextTexture = SDL_CreateTextureFromSurface(renderer, finaleTextSurface);
			SDL_RenderCopy(renderer, finaleTextTexture, NULL, &win_banner);
			SDL_FreeSurface(finaleTextSurface);
			SDL_DestroyTexture(finaleTextTexture);
			stage_is_running = false;
			menu_is_running = true;
		}
		if (lose)
		{
			SDL_Rect lose_banner = {
				(int)0,
				(int)WINDOW_HEIGHT / 3,
				(int)WINDOW_WIDTH,
				(int)100
			};
			length = snprintf(NULL, 0, "%s", "Você perdeu!");
			aux = malloc(length + 1);
			snprintf(aux, length + 1, "%s", "Você perdeu!");

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 127);
			SDL_RenderFillRect(renderer, &lose_banner);

			finaleTextSurface = TTF_RenderText_Blended(font, aux, color);
			finaleTextTexture = SDL_CreateTextureFromSurface(renderer, finaleTextSurface);
			SDL_RenderCopy(renderer, finaleTextTexture, NULL, &lose_banner);
			SDL_FreeSurface(finaleTextSurface);
			SDL_DestroyTexture(finaleTextTexture);
			stage_is_running = false;
			menu_is_running = true;
		}
		SDL_RenderPresent(renderer);
	}	
}

// Closes window and terminate process.
void destroy_window()
{
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

// Main function.
int main(int argc, char* argv[])
{
	// If everything is initialized, game_is_running equals true.
	game_is_running = initialize_window();

	menu_is_running = game_is_running;
		
	while (game_is_running)
	{
		setup_menu();
		while (menu_is_running)
		{
			process_input(); // Process user mouse/keyboard inputs in menu.
			update(); // Process game objects states in menu.
			render(); // Process object rendering in menu.
		}
		setup_stage(10, 10, 10); // Initializes minefield parameters.
		while (stage_is_running)
		{
			process_input(); // Process user mouse/keyboard inputs in stage.
			update(); // Process game objects states in stage.
			render(); // Process object rendering in stage.
		}		
	}
	
	destroy_window(); // Closes window and terminate process.
	return 0;
}