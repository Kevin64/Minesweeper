#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "initField.h"
#include "fillField.h"
#include "printEngine.h"
#include "openSpaceField.h"
#include "game.h"
#include "constants.h"

// SDL global variables.
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;
SDL_Color colorTip = COLOR_TIP;
SDL_Color colorTitle = COLOR_TITLE;
SDL_Color colorSelectInput = COLOR_SEL_INPT;
TTF_Font *font;
SDL_Surface *menuTextSurface1, *menuTextSurface2, *finaleTextSurface, *gameTextSurface, *titleTextSurface, *widthTextSurface;
SDL_Texture *menuTextTexture1, *menuTextTexture2, *finaleTextTexture, *gameTextTexture, *titleTextTexture, *widthTextTexture;
SDL_Rect new_game_button, quit_game_button, tile_square, width_field_input_form, width_field_text_form, height_field_input_form, height_field_text_form, mine_amount_input_form, mine_amount_text_form, ok_button_form;

// Game global variables.
bool game_is_running = false;
bool stage_is_running = false;
bool main_menu_is_running = false;
bool select_menu_is_running = false;
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
float centerFieldX, centerFieldY, centerFormTextX, centerFormInputX;
char *aux;
char *paramInput;
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
void setup_main_menu()
{
	// Sets button size.
	button_x = BUTTON_X;
	button_y = BUTTON_Y;
	button_w = BUTTON_W;
	button_h = BUTTON_H;

	// New game button position.
	new_game_button.x = button_x + ((WINDOW_WIDTH / 2) - button_w / 2);
	new_game_button.y = button_y + ((WINDOW_HEIGHT / 2) - button_h / 2);
	new_game_button.w = button_w;
	new_game_button.h = button_h;
	
	// Quit game button position.
	quit_game_button.x = new_game_button.x;
	quit_game_button.y = new_game_button.y + BUTTON_SPACING;
	quit_game_button.w = button_w;
	quit_game_button.h = button_h;

	paramInput = malloc(2);
}

// Initializes game parameters.
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
	stage_is_running = true;
}

// When a game is finished, waits 3 seconds before going back to the main menu.
void setup_new_game()
{
	if (win || lose)
	{
		Sleep(3000);
		main_menu_is_running = true;
		stage_is_running = false;
	}	
}

// Process user mouse/keyboard inputs in menu.
void process_input()
{
	SDL_StartTextInput();
	SDL_SetTextInputRect(&width_field_input_form);
	while (SDL_PollEvent(&event))
	{
		// If main menu is running...
		if (main_menu_is_running && !select_menu_is_running && !stage_is_running)
		{			
			// ... wait for an input event.
			switch (event.type)
			{
			// If X button on the window is clicked, quits.
			case SDL_QUIT:
				game_is_running = false;
				main_menu_is_running = false;
				select_menu_is_running = false;
				stage_is_running = false;
				break;
			// If a key is pressed...
			case SDL_KEYDOWN:
				// ... and it's the Escape key, quits the game.
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					game_is_running = false;
					main_menu_is_running = false;
					select_menu_is_running = false;
					stage_is_running = false;
				}
				// ... and it's the down arrow, go to button below.
				if (event.key.keysym.sym == SDLK_DOWN && option < 1)
					option++;
				// ... and it's the up arrow, go to button above.
				if (event.key.keysym.sym == SDLK_UP && option > 0)
					option--;
				// ... and it's the Return, selects...
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					// ... the new game button.
					if (option == 0)
					{
						//main_menu_is_running = false;
						paramInput = event.text.text;
					}
					// ... the quit game button.
					else if (option == 1)
					{
						game_is_running = false;
						main_menu_is_running = false;
						select_menu_is_running = false;
						stage_is_running = false;
					}
				}
				break;
			// If the left mouse button is pressed, activates its variable.
			case SDL_MOUSEBUTTONDOWN:
				clickedL = event.button.button == SDL_BUTTON_LEFT;
				break;
			// If the left mouse button is released, deactivates its variable.
			case SDL_MOUSEBUTTONUP:
				clickedL = !event.button.button == SDL_BUTTON_LEFT;
				break;
			}			
		}
		// If select menu is running...
		else if (!main_menu_is_running && select_menu_is_running && !stage_is_running)
		{
			switch (event.type)
			{
				// If X button on the window is clicked, quits.
			case SDL_QUIT:
				game_is_running = false;
				main_menu_is_running = false;
				select_menu_is_running = false;
				stage_is_running = false;
				break;
				// If a key is pressed...
			case SDL_KEYDOWN:
				// ... and it's the Escape key, returns to main menu.
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					main_menu_is_running = true;
					select_menu_is_running = false;
					stage_is_running = false;
				}
				break;
			}
		}
		// If game is running...
		else if (!main_menu_is_running && !select_menu_is_running && stage_is_running)
		{			
			// ... wait for an input event.
			switch (event.type)
			{
			// If X button on the window is clicked, quits.
			case SDL_QUIT:
				game_is_running = false;
				main_menu_is_running = false;
				select_menu_is_running = false;
				stage_is_running = false;
				break;
			// If a key is pressed...
			case SDL_KEYDOWN:
				// ... and it's the Escape key, returns to main menu.
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					main_menu_is_running = true;
					select_menu_is_running = false;
					stage_is_running = false;
				}
				break;
			// If the left or right mouse button is pressed, activates their variables.
			case SDL_MOUSEBUTTONDOWN:
				clickedL = event.button.button == SDL_BUTTON_LEFT;
				clickedR = event.button.button == SDL_BUTTON_RIGHT;
				break;
			// If the left or right mouse button is released, deactivates their variables.
			case SDL_MOUSEBUTTONUP:
				clickedL = !event.button.button == SDL_BUTTON_LEFT;
				clickedR = !event.button.button == SDL_BUTTON_RIGHT;
				break;
			}			
		}
	}
	SDL_StopTextInput();
	SDL_GetMouseState(&xm, &ym);	
}

// Process game objects states in game.
void update()
{
	// Sleep the execution until we reach the target frame time in milliseconds.
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

	// Only call delay if we are too fast to process this frame.
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);

	// Get a delta time factor converted to seconds to be used to update my objects.
	delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();

	//TODO: Here is where we can update the state of our objects
	
	// If both lower and upper fields are still allocated, player can open/flag tiles, and then game checks to victory or defeat.
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
	// If main menu is running.
	if (!stage_is_running && main_menu_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		printTitle(renderer, titleTextSurface, titleTextTexture, font, colorTitle);

		// If mouse is over new game button, highlights itself and if clicked, go to select size/mines screen.
		if (xm >= new_game_button.x && xm <= new_game_button.x + new_game_button.w && ym >= new_game_button.y && ym <= new_game_button.y + new_game_button.h)
		{
			option = 0;
			if (clickedL)
			{
				main_menu_is_running = false;
				select_menu_is_running = true;
				clickedL = false;
			}
		}
		// Else if mouse is over quit game button, highlights itself and if clicked, quits the game.
		else if (xm >= quit_game_button.x && xm <= quit_game_button.x + quit_game_button.w && ym >= quit_game_button.y && ym <= quit_game_button.y + quit_game_button.h)
		{
			option = 1;
			if (clickedL)
			{
				game_is_running = false;
				main_menu_is_running = false;
				select_menu_is_running = false;
				stage_is_running = false;
				clickedL = false;
			}
		}

		// Highlisghts new game button.
		if (option == 0)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &new_game_button);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &quit_game_button);
		}
		// Highlisghts quit game button.
		else if (option == 1)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &quit_game_button);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &new_game_button);
		}

		SDL_RenderPresent(renderer);
	}
	// If select menu is running.
	else if (!stage_is_running && !main_menu_is_running && select_menu_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		printTitle(renderer, titleTextSurface, titleTextTexture, font, colorTitle);
		
		width_field_text_form.y = 0;
		width_field_text_form.w = WINDOW_WIDTH / 3;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (width_field_text_form.w) / 2); // Centralizes the text/form/button at the center of the window width.
		width_field_text_form.h = 80;
		width_field_text_form.x = centerFormTextX;
		
		width_field_input_form.y = 100;
		width_field_input_form.w = WINDOW_WIDTH / 6;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (width_field_input_form.w) / 2); // Centralizes the text/form/button at the center of the window width.
		width_field_input_form.h = 80;
		width_field_input_form.x = centerFormInputX;

		height_field_text_form.y = 200;
		height_field_text_form.w = WINDOW_WIDTH / 3;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (height_field_text_form.w) / 2); // Centralizes the text/form/button at the center of the window width.
		height_field_text_form.h = 80;
		height_field_text_form.x = centerFormTextX;
		
		height_field_input_form.y = 300;
		height_field_input_form.w = WINDOW_WIDTH / 6;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (height_field_input_form.w) / 2); // Centralizes the text/form/button at the center of the window width.
		height_field_input_form.h = 80;
		height_field_input_form.x = centerFormInputX;

		mine_amount_text_form.y = 400;
		mine_amount_text_form.w = WINDOW_WIDTH / 3;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (mine_amount_text_form.w) / 2); // Centralizes the text/form/button at the center of the window width.
		mine_amount_text_form.h = 80;
		mine_amount_text_form.x = centerFormTextX;

		mine_amount_input_form.y = 500;
		mine_amount_input_form.w = WINDOW_WIDTH / 6;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (mine_amount_input_form.w) / 2); // Centralizes the text/form/button at the center of the window width.
		mine_amount_input_form.h = 80;
		mine_amount_input_form.x = centerFormInputX;

		ok_button_form.y = 600;
		ok_button_form.w = WINDOW_WIDTH / 6;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (ok_button_form.w) / 2); // Centralizes the text/form/button at the center of the window width.
		ok_button_form.h = 100;
		ok_button_form.x = centerFormTextX;

		//SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorTitle, width_field_text_form, WIDTH_TEXT);
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorTitle, height_field_text_form, HEIGHT_TEXT);
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorTitle, mine_amount_text_form, MINE_AMOUNT_TEXT);
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorTitle, ok_button_form, OK_BUTTON);
		/*SDL_RenderFillRect(renderer, &width_field_text_form);
		SDL_RenderFillRect(renderer, &height_field_text_form);
		SDL_RenderFillRect(renderer, &mine_amount_text_form);*/

		SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
		SDL_RenderFillRect(renderer, &width_field_input_form);		
		SDL_RenderFillRect(renderer, &height_field_input_form);		
		SDL_RenderFillRect(renderer, &mine_amount_input_form);
		SDL_RenderFillRect(renderer, &ok_button_form);

		SDL_RenderPresent(renderer);
	}
	// If game is running.
	else if (stage_is_running && !main_menu_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		printTitle(renderer, titleTextSurface, titleTextTexture, font, colorTitle);

		i = 0;
		j = 0;
		
		// Centralizes the field at the center of the window.
		centerFieldX = ((WINDOW_WIDTH / 2) - (c->x * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);
		centerFieldY = ((WINDOW_HEIGHT / 2) - (c->y * (TILE_SIDE_SIZE + TILE_SPACING)) / 2);

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

					gameTextSurface = TTF_RenderText_Blended(font, aux, colorTip);
					gameTextTexture = SDL_CreateTextureFromSurface(renderer, gameTextSurface);
					SDL_RenderCopy(renderer, gameTextTexture, NULL, &tile_square);
					SDL_FreeSurface(gameTextSurface);
					SDL_DestroyTexture(gameTextTexture);
				}
			}
		}
		// If player wins, show a victory banner and deallocates lower and upper fields.
		if (win)
		{
			printFinish(f, c, renderer, finaleTextSurface, finaleTextTexture, font, colorTip, true);
			free(f);
			f = NULL;
			free(c);
			c = NULL;
		}
		// If player wins, show a defeat banner and deallocates lower and upper fields.
		if (lose)
		{
			printFinish(f, c, renderer, finaleTextSurface, finaleTextTexture, font, colorTip, false);
			free(f);
			f = NULL;
			free(c);
			c = NULL;
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

	main_menu_is_running = game_is_running;
		
	while (game_is_running)
	{
		setup_main_menu();
		while (main_menu_is_running)
		{
			process_input(); // Process user mouse/keyboard inputs in menu.
			update(); // Process game objects states in menu.
			render(); // Process object rendering in menu.
		}
		while (select_menu_is_running)
		{
			process_input(); // Process user mouse/keyboard inputs in menu.
			update(); // Process game objects states in menu.
			render(); // Process object rendering in menu.
		}

		//if(game_is_running)
		//	setup_stage(10, 10, 10); // Initializes minefield parameters.

		while (stage_is_running)
		{
			process_input(); // Process user mouse/keyboard inputs in stage.
			update(); // Process game objects states in stage.
			render(); // Process object rendering in stage.
			setup_new_game();
		}		
	}
	
	destroy_window(); // Closes window and terminate process.
	return 0;
}