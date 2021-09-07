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

// SDL global variables.
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;
SDL_Color color;
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
bool selected = false;
int last_frame_time = 0;
int length, option = 0;
int i, j;
int ij_sel[3];
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
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	// Create window with those parameters.
	window = SDL_CreateWindow(
		"Minesweeper", // Title in titlebar.
		SDL_WINDOWPOS_CENTERED, // Screen X position.
		SDL_WINDOWPOS_CENTERED, // Screen Y position.
		WINDOW_WIDTH, // Window width.
		WINDOW_HEIGHT, // Window height.
		SDL_WINDOW_ALLOW_HIGHDPI // Flag for HiDPI support.
	);
	// If can't create window, outputs an error message.
	if (!window)
	{
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}
	// Initializes SDL renderer.
	renderer = SDL_CreateRenderer(window, -1, 0);
	// If can't create renderer, outputs an error message.
	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return false;
	}

	// Initializes the font through SDL_TTF.
	if (TTF_Init() != 0)
	{
		fprintf(stderr, "Error initializing SDL_TTF.\n");
		return false;
	}
	font = TTF_OpenFont("C:\\tahoma.ttf" /*path*/, 48 /*size*/);
	
	// Sets the alpha channel for blending.
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	// Returns true if all is ok.
	return true;
}

// Initializes menu parameters.
void setup_menu()
{
	button_x = 0;
	button_y = WINDOW_HEIGHT;
	button_w = WINDOW_WIDTH;
	button_h = 100;

	new_game_button.x = button_x;
	new_game_button.y = button_y / 4;
	new_game_button.w = button_w;
	new_game_button.h = button_h;
	
	quit_game_button.x = button_x;
	quit_game_button.y = button_y / 2;
	quit_game_button.w = button_w;
	quit_game_button.h = button_h;
}
// Initializes minefield parameters.
void setup_stage(int w, int h, int m)
{
	tile.x = TILE_SPACING;
	tile.y = TILE_SPACING;
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

// Process user mouse/keyboard inputs in game.
void process_input_menu()
{
	while (SDL_PollEvent(&event))
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
			{
				option++;
				printf("%d", option);
			}
			if (event.key.keysym.sym == SDLK_UP && option > 0)
			{
				option--;
				printf("%d", option);
			}
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
		}

		SDL_GetMouseState(&xm, &ym);

		if (event.button.button == SDL_BUTTON_LEFT)
			clickedL = true;
	}
}

// Process user mouse/keyboard inputs in stage.
void process_input_stage()
{
	while (SDL_PollEvent(&event))
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
		}

		SDL_GetMouseState(&xm, &ym);

		if (event.button.button == SDL_BUTTON_LEFT)
			clickedL = true;
		if (event.type == SDL_MOUSEBUTTONDOWN)
			clickedR = true;
	}
}

// Process game objects states in menu.
void update_menu()
{
	// Sleep the execution until we reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

	// Only call delay if we are too fast to process this frame
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);

	// Get a delta time factor converted to seconds to be used to update my objects
	delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();
}

// Process game objects states in stage.
void update_stage()
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
	
	win = checkWin(f, c);
	lose = checkLose(f, c, ij_sel);
	
}

// Process object rendering in menu.
void render_menu()
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

// Process object rendering in stage.
void render_stage()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	i = 0;
	j = 0;

	//TODO: Here is where we can start drawing ou game objects

	for (i = 0; i < c->x; i++)
	{
		for (j = 0; j < c->y; j++)
		{			
			xi = (tile.x + tile.w) * j;
			yi = (tile.y + tile.h) * i;
			xf = (xi + tile.w);
			yf = (yi + tile.h);

			tile_square.x = (tile.x + tile.w) * j;
			tile_square.y = (tile.y + tile.h) * i;
			tile_square.w = tile.w;
			tile_square.h = tile.h;

			if (xm >= xi && xm <= xf && ym >= yi && ym <= yf && c->mat[i][j] != EDGE_L_R && c->mat[i][j] != EDGE_T_B)
			{
				if (clickedL)
				{
					openField(f, c, i, j, OPEN_F);
					clickedL = false;
					clickedR = false;
					ij_sel[0] = i;
					ij_sel[1] = j;
					ij_sel[2] = OPEN_F;
				}
				if (clickedR)
				{
					openField(f, c, i, j, FLAG_F);
					clickedL = false;
					clickedR = false;
				}
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
				SDL_SetRenderDrawColor(renderer, 153, 204, 255, 255);
				SDL_RenderFillRect(renderer, &tile_square);
			}
			else if (c->mat[i][j] == 0)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &tile_square);
			}
			else
			{
				length = snprintf(NULL, 0, "%d", c->mat[i][j]);
				aux = malloc(length + 1);
				snprintf(aux, length + 1, "%d", c->mat[i][j]);

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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
	// If everything is initialized, game_is_running equals true.
	game_is_running = initialize_window();

	menu_is_running = game_is_running;
		
	while (game_is_running)
	{
		setup_menu();
		while (menu_is_running)
		{
			process_input_menu(); // Process user mouse/keyboard inputs in menu.
			update_menu(); // Process game objects states in menu.
			render_menu(); // Process object rendering in menu.
		}
		setup_stage(10, 10, 10); // Initializes minefield parameters.
		while (stage_is_running)
		{
			process_input_stage(); // Process user mouse/keyboard inputs in stage.
			update_stage(); // Process game objects states in stage.
			render_stage(); // Process object rendering in stage.
		}		
	}
	// Closes window and terminate process.
	destroy_window();
	return 0;
}