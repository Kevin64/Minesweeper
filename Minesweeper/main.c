#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "initField.h"
#include "fillField.h"
#include "printEngine.h"
#include "sumMineField.h"
#include "openSpaceField.h"
#include "game.h"
#include "constants.h"

// SDL global variables.
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;
SDL_Color colorTip = COLOR_TIP;
SDL_Color colorTitle = COLOR_TITLE;
SDL_Color colorForm = COLOR_FORM;
SDL_Color colorButton = COLOR_BUTTON;
SDL_Color colorSelectInput = COLOR_SEL_INPT;
SDL_Color colorAlert = COLOR_ALERT;
SDL_Color colorMenuText = COLOR_MENU_TEXT;
TTF_Font *font;
Mix_Music *backgroundMusicMenu, *backgroundMusicStage;
Mix_Chunk *soundEffectL, *soundEffectR, *soundEffectMine, *soundEffectMenu, *soundEffectVictory;
SDL_Surface *menuTextSurface1, *menuTextSurface2, *finaleTextSurface, *gameTextSurface, *titleTextSurface, *widthTextSurface, *widthInputSurface, *heightInputSurface, *mineInputSurface, *alertTextSurface, *windowIconSurface, *mineIconSurface, *mineBoomIconSurface, *mineDeathIconSurface, *flagIconSurface, *edgeIconSurface, *coverIconSurface;
SDL_Texture *menuTextTexture1, *menuTextTexture2, *finaleTextTexture, *gameTextTexture, *titleTextTexture, *widthTextTexture, *widthInputTexture, *heightInputTexture, *mineInputTexture, *alertTextTexture, *windowIconTexture, *mineIconTexture, *mineBoomIconTexture, *mineDeathIconTexture, *flagIconTexture, *edgeIconTexture, *coverIconTexture;
SDL_Rect new_game_button, quit_game_button, tile_square, width_field_textbox, width_field_label, height_field_textbox, height_field_label, mine_amount_textbox, mine_amount_label, ok_button_rect;

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
bool showMines = false;
bool resetIJ = true;
bool soundPlayed = false;
int last_frame_time = 0;
int length, option = 0, formField = 0;
int alpha1 = ALPHA_UNSELECTED, alpha2 = ALPHA_UNSELECTED, alpha3 = ALPHA_UNSELECTED, alpha4 = ALPHA_UNSELECTED;
int i, j, counter1 = 1, counter2 = 1, counter3 = 1;
int ij_selected[3];
int xm, ym, xi, xf, yi, yf;
int button_x, button_y, button_w, button_h;
int w, h, m;
int centerFieldX, centerFieldY, centerFormTextX, centerFormInputX;
float delta_time = 0.0f;
char *aux;
char paramInput1[3], paramInput2[3], paramInput3[4];
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

	// Sets the window icon.
	windowIconSurface = IMG_Load(WINDOW_ICON);
	SDL_SetWindowIcon(window, windowIconSurface);

	// Sets icons for assets.
	mineBoomIconSurface = IMG_Load(MINE_BOOM_ICON);
	mineDeathIconSurface = IMG_Load(MINE_DEATH_ICON);
	edgeIconSurface = IMG_Load(EDGE_ICON);
	flagIconSurface = IMG_Load(FLAG_ICON);
	coverIconSurface = IMG_Load(COVER_ICON);
	
	// If can't create audio, outputs an error message.
	if (Mix_OpenAudio(44800, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		fprintf(stderr, ERROR_SDL_AUDIO);
		return false;
	}
	backgroundMusicMenu = Mix_LoadMUS(BACKGROUND_MUSIC_MENU); // Loads background music in menus.
	backgroundMusicStage = Mix_LoadMUS(BACKGROUND_MUSIC_STAGE); // Loads background music in stage.
	soundEffectL = Mix_LoadWAV(SOUND_EFFECT_L); // Loads sound effect for left mouse click.
	soundEffectR = Mix_LoadWAV(SOUND_EFFECT_R); // Loads sound effect for right mouse click.
	soundEffectMenu = Mix_LoadWAV(SOUND_EFFECT_MENU); // Loads sound effect for menu hover/click.
	soundEffectMine = Mix_LoadWAV(SOUND_EFFECT_MINE); // Loads sound effect for defeat.
	soundEffectVictory = Mix_LoadWAV(SOUND_EFFECT_VICTORY); // Loads sound effect for victory.

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
}

// When a game is finished, waits 3 seconds ans stops music before going back to the main menu.
void wait_interval()
{
	if (win || lose)
	{
		SDL_Delay(5000);
		main_menu_is_running = true;
		stage_is_running = false;
		Mix_HaltMusic();
	}	
}

// Process user mouse/keyboard inputs in menu.
void process_input()
{
	SDL_StartTextInput();	
	while (SDL_PollEvent(&event))
	{
		// If main menu is running...
		if (main_menu_is_running && !select_menu_is_running && !stage_is_running)
		{			
			showMines = false;
			resetIJ = true;
			win = false;
			lose = false;
			ij_selected[0] = NULL;
			ij_selected[1] = NULL;
			ij_selected[2] = NULL;
			counter1 = counter2 = counter3 = formField = 0;
			memset(paramInput1, 0, sizeof paramInput1);
			memset(paramInput2, 0, sizeof paramInput2);
			memset(paramInput3, 0, sizeof paramInput3);

			// If background menu music is not playing, starts to play.
			if (!Mix_PlayingMusic())
				Mix_PlayMusic(backgroundMusicMenu, -1);

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
						main_menu_is_running = false;
						select_menu_is_running = true;
						stage_is_running = false;
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
				if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(paramInput1) > 0 && formField == 0)
				{
					substring(paramInput1, paramInput1, 0, strlen(paramInput1) - 1);
					counter1 -= TEXT_BOX_FINE_ADJUSTEMENT;
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(paramInput2) > 0 && formField == 1)
				{
					substring(paramInput2, paramInput2, 0, strlen(paramInput2) - 1);
					counter2 -= TEXT_BOX_FINE_ADJUSTEMENT;
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(paramInput3) > 0 && formField == 2)
				{
					substring(paramInput3, paramInput3, 0, strlen(paramInput3) - 1);
					counter3 -= TEXT_BOX_FINE_ADJUSTEMENT;
				}
				break;
			case SDL_TEXTINPUT:
				if (event.text.text[0] != '0' && event.text.text[0] != '1' && event.text.text[0] != '2' && event.text.text[0] != '3' && event.text.text[0] != '4' && event.text.text[0] != '5' && event.text.text[0] != '6' && event.text.text[0] != '7' && event.text.text[0] != '8' && event.text.text[0] != '9');
				else
				{
					if (paramInput1[1] == '\0' && formField == 0)
					{
						SDL_SetTextInputRect(&width_field_label);
						strcat(paramInput1, event.text.text);
						counter1 += TEXT_BOX_FINE_ADJUSTEMENT;
					}
					else if (paramInput2[1] == '\0' && formField == 1)
					{
						SDL_SetTextInputRect(&width_field_label);
						strcat(paramInput2, event.text.text);
						counter2 += TEXT_BOX_FINE_ADJUSTEMENT;
					}
					else if (paramInput3[2] == '\0' && formField == 2)
					{
						SDL_SetTextInputRect(&width_field_label);
						strcat(paramInput3, event.text.text);
						counter3 += TEXT_BOX_FINE_ADJUSTEMENT;
					}
				}
				
				break;
				// If the left or right mouse button is pressed, activates their variables.
			case SDL_MOUSEBUTTONDOWN:
				clickedL = event.button.button == SDL_BUTTON_LEFT;
				break;
				// If the left or right mouse button is released, deactivates their variables.
			case SDL_MOUSEBUTTONUP:
				clickedL = !event.button.button == SDL_BUTTON_LEFT;
				break;
			}
		}
		// If stage is running...
		else if (!main_menu_is_running && !select_menu_is_running && stage_is_running)
		{		
			// If background stage music is not playing, starts to play.
			if (!Mix_PlayingMusic())
				Mix_PlayMusic(backgroundMusicStage, -1);

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
					Mix_HaltMusic(); // Stops playing stage music.
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
	if (main_menu_is_running && !select_menu_is_running && !stage_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		printTitle(renderer, titleTextSurface, titleTextTexture, font, colorTitle);

		// If mouse is over new game button, highlights itself and if clicked, go to select size/mines screen.
		if (xm >= new_game_button.x && xm <= new_game_button.x + new_game_button.w && ym >= new_game_button.y && ym <= new_game_button.y + new_game_button.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the button area.
			if (!soundPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundPlayed = true;
			}			
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
			// If sound effect has not been played yet, plays once if the mouse stays inside the button area.
			if (!soundPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundPlayed = true;
			}
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
		else
			soundPlayed = false; // If the mouse exits any button area, sets sound effect to play again on collision.

		// Highlisghts new game button.
		if (option == 0)
		{
			printFormText(renderer, menuTextSurface1, menuTextTexture1, font, colorMenuText, new_game_button, NEW_GAME_TEXT, 0, 255, 0, 255);
			printFormText(renderer, menuTextSurface2, menuTextTexture2, font, colorMenuText, quit_game_button, QUIT_GAME_TEXT, 96, 0, 0, 96);
		}
		// Highlisghts quit game button.
		else if (option == 1)
		{
			printFormText(renderer, menuTextSurface2, menuTextTexture2, font, colorMenuText, new_game_button, NEW_GAME_TEXT, 96, 0, 0, 96);
			printFormText(renderer, menuTextSurface2, menuTextTexture2, font, colorMenuText, quit_game_button, QUIT_GAME_TEXT, 0, 255, 0, 255);
		}

		SDL_RenderPresent(renderer);
	}
	// If select menu is running.
	else if (!main_menu_is_running && select_menu_is_running && !stage_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		printTitle(renderer, titleTextSurface, titleTextTexture, font, colorTitle);

		width_field_label.y = WIDTH_TEXT_Y;
		width_field_label.w = WIDTH_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (width_field_label.w) / 2); // Centralizes the text/form/button at the center of the window width.
		width_field_label.h = WIDTH_TEXT_H;
		width_field_label.x = centerFormTextX;
		
		width_field_textbox.y = WIDTH_INPUT_Y;
		width_field_textbox.w = WIDTH_INPUT_W + counter1;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (width_field_textbox.w) / 2); // Centralizes the text/form/button at the center of the window width.
		width_field_textbox.h = WIDTH_INPUT_H;
		width_field_textbox.x = centerFormInputX;
		printFormText(renderer, widthInputSurface, widthInputTexture, font, colorForm, width_field_textbox, paramInput1, 0, 0, 0, 255);

		height_field_label.y = HEIGHT_TEXT_Y;
		height_field_label.w = HEIGHT_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (height_field_label.w) / 2); // Centralizes the text/form/button at the center of the window width.
		height_field_label.h = HEIGHT_TEXT_H;
		height_field_label.x = centerFormTextX;
		
		height_field_textbox.y = HEIGHT_INPUT_Y;
		height_field_textbox.w = HEIGHT_INPUT_W + counter2;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (height_field_textbox.w) / 2); // Centralizes the text/form/button at the center of the window width.
		height_field_textbox.h = HEIGHT_INPUT_H;
		height_field_textbox.x = centerFormInputX;
		printFormText(renderer, heightInputSurface, heightInputTexture, font, colorForm, height_field_textbox, paramInput2, 0, 0, 0, 255);

		mine_amount_label.y = MINE_TEXT_Y;
		mine_amount_label.w = MINE_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (mine_amount_label.w) / 2); // Centralizes the text/form/button at the center of the window width.
		mine_amount_label.h = MINE_TEXT_H;
		mine_amount_label.x = centerFormTextX;

		mine_amount_textbox.y = MINE_INPUT_Y;
		mine_amount_textbox.w = MINE_INPUT_W + counter3;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (mine_amount_textbox.w) / 2); // Centralizes the text/form/button at the center of the window width.
		mine_amount_textbox.h = MINE_INPUT_H;
		mine_amount_textbox.x = centerFormInputX;
		printFormText(renderer, mineInputSurface, mineInputTexture, font, colorForm, mine_amount_textbox, paramInput3, 0, 0, 0, 255);

		ok_button_rect.y = OK_BUTTON_Y;
		ok_button_rect.w = OK_BUTTON_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (ok_button_rect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		ok_button_rect.h = OK_BUTTON_H;
		ok_button_rect.x = centerFormTextX;

		// Draws each text label.
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorForm, width_field_label, WIDTH_TEXT, 0, 0, 0, 255);
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorForm, height_field_label, HEIGHT_TEXT, 0, 0, 0, 255);
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorForm, mine_amount_label, MINE_AMOUNT_TEXT, 0, 0, 0, 255);
		printFormText(renderer, widthTextSurface, widthTextTexture, font, colorButton, ok_button_rect, OK_BUTTON_TEXT, 0, 180, 30, alpha4);

		// Draws each textbox for input.
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha1);
		SDL_RenderFillRect(renderer, &width_field_textbox);
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha2);
		SDL_RenderFillRect(renderer, &height_field_textbox);
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha3);
		SDL_RenderFillRect(renderer, &mine_amount_textbox);


		// If the mouse is over the width textbox, and a click is detected, selects that box for input.
		if (xm >= width_field_textbox.x && xm <= width_field_textbox.x + width_field_textbox.w && ym >= width_field_textbox.y && ym <= width_field_textbox.y + width_field_textbox.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the textbox area.
			if (!soundPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundPlayed = true;
			}
			if (clickedL)
				formField = 0;
		}
		// If the mouse is over the height textbox, and a click is detected, selects that box for input.
		else if (xm >= height_field_textbox.x && xm <= height_field_textbox.x + height_field_textbox.w && ym >= height_field_textbox.y && ym <= height_field_textbox.y + height_field_textbox.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the textbox area.
			if (!soundPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundPlayed = true;
			}
			if (clickedL)
				formField = 1;
		}
		// If the mouse is over the mine textbox, and a click is detected, selects that box for input.
		else if (xm >= mine_amount_textbox.x && xm <= mine_amount_textbox.x + mine_amount_textbox.w && ym >= mine_amount_textbox.y && ym <= mine_amount_textbox.y + mine_amount_textbox.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the textbox area.
			if (!soundPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundPlayed = true;
			}
			if (clickedL)
				formField = 2;
		}
		// If the mouse is over the ok button, and a click is detected, starts a new stage.
		else if (xm >= ok_button_rect.x && xm <= ok_button_rect.x + ok_button_rect.w && ym >= ok_button_rect.y && ym <= ok_button_rect.y + ok_button_rect.h)
		{
			alpha4 = ALPHA_SELECTED * 2; // Highlights OK button.
			// If sound effect has not been played yet, plays once if the mouse stays inside the button area.
			if (!soundPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundPlayed = true;
			}
			if (clickedL)
				formField = 3;
		}
		else
			soundPlayed = false; // If the mouse exits any textbox/button area, sets sound effect to play again on collision.
		
		// Highlights the width textbox.
		if (formField == 0)
		{
			alpha1 = ALPHA_SELECTED;
			alpha2 = ALPHA_UNSELECTED;
			alpha3 = ALPHA_UNSELECTED;
		}
		// Highlights the height textbox.
		if (formField == 1)
		{
			alpha1 = ALPHA_UNSELECTED;
			alpha2 = ALPHA_SELECTED;
			alpha3 = ALPHA_UNSELECTED;
		}
		// Highlights the mine textbox.
		if (formField == 2)
		{
			alpha1 = ALPHA_UNSELECTED;
			alpha2 = ALPHA_UNSELECTED;
			alpha3 = ALPHA_SELECTED;
		}
		// Grab width, height and mine amount and starts a new stage.
		if (formField == 3)
		{
			// Converts each string to an integer.
			w = strtol(paramInput1, NULL, 10);
			h = strtol(paramInput2, NULL, 10);
			m = strtol(paramInput3, NULL, 10);
			if (w > WIDTH_MIN && w < WIDTH_MAX && h > HEIGHT_MIN && h < HEIGHT_MAX && m > MINE_MIN && m < (w * h))
			{
				select_menu_is_running = false;
				stage_is_running = true;
				Mix_HaltMusic(); // Stops background menu music.
			}
			else
				printAlert(renderer, alertTextSurface, alertTextTexture, font, colorAlert);
		}

		SDL_RenderPresent(renderer);
	}
	// If game is running.
	else if (!main_menu_is_running && !select_menu_is_running && stage_is_running)
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
				xi = (tile.x + tile.w) * i + centerFieldX + X_FINE_ADJUSTEMENT;
				yi = (tile.y + tile.h) * j + centerFieldY + Y_FINE_ADJUSTEMENT;
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
						// If cover has not been opened yet, plays the left click sound.
						if(c->mat[i][j] != f->mat[i][j])
							Mix_PlayChannel(-1, soundEffectL, 0);
						canOpen = true;
						ij_selected[0] = i;
						ij_selected[1] = j;
						ij_selected[2] = OPEN_F;
					}
					if (clickedR)
					{
						// If cover has not been opened yet, plays the right click sound.
						if (c->mat[i][j] != f->mat[i][j])
							Mix_PlayChannel(-1, soundEffectR, 0);
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
					f->mat[i][j] = MINE_TRIG;

					// Shows a custom icon for mines.					
					mineDeathIconTexture = SDL_CreateTextureFromSurface(renderer, mineDeathIconSurface);
					SDL_RenderCopy(renderer, mineDeathIconTexture, NULL, &tile_square);
					SDL_DestroyTexture(mineDeathIconTexture);
					showMines = true;
					if (resetIJ == true)
					{
						i = j = 0;
						resetIJ = false;
						break;
					}
				}
				else if (c->mat[i][j] == EDGE_L_R || c->mat[i][j] == EDGE_T_B)
				{
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
					SDL_RenderFillRect(renderer, &tile_square);

					// Shows a custom icon for edges.					
					edgeIconTexture = SDL_CreateTextureFromSurface(renderer, edgeIconSurface);
					SDL_RenderCopy(renderer, edgeIconTexture, NULL, &tile_square);
					SDL_DestroyTexture(edgeIconTexture);
				}
				else if (c->mat[i][j] == FLAG)
				{
					SDL_SetRenderDrawColor(renderer, 127, 255, 0, 255);
					SDL_RenderFillRect(renderer, &tile_square);

					// Shows a custom icon for flags.					
					flagIconTexture = SDL_CreateTextureFromSurface(renderer, flagIconSurface);
					SDL_RenderCopy(renderer, flagIconTexture, NULL, &tile_square);
					SDL_DestroyTexture(flagIconTexture);
				}
				else if (c->mat[i][j] == COVER)
				{
					SDL_SetRenderDrawColor(renderer, 64, 64, 128, 255);
					SDL_RenderFillRect(renderer, &tile_square);

					// Shows a custom icon for cover.					
					coverIconTexture = SDL_CreateTextureFromSurface(renderer, coverIconSurface);
					SDL_RenderCopy(renderer, coverIconTexture, NULL, &tile_square);
					SDL_DestroyTexture(coverIconTexture);
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

				// If the player dies, show all the hidden mines.
				if (f->mat[i][j] == MINE && showMines)
				{
					// Shows a custom icon for mines.					
					mineBoomIconTexture = SDL_CreateTextureFromSurface(renderer, mineBoomIconSurface);
					SDL_RenderCopy(renderer, mineBoomIconTexture, NULL, &tile_square);
					SDL_DestroyTexture(mineBoomIconTexture);
				}
			}
		}

		// If player wins, show a victory banner and deallocates lower and upper fields.
		if (win)
		{
			Mix_PlayChannel(-1, soundEffectVictory, 0); // Plays a victory sound.
			printFinish(f, c, renderer, finaleTextSurface, finaleTextTexture, font, colorTip, true);
			free(f);
			f = NULL;
			free(c);
			c = NULL;
			stage_is_running = false;
		}
		// If player wins, show a defeat banner and deallocates lower and upper fields.
		if (lose)
		{
			Mix_PlayChannel(-1, soundEffectMine, 0); // PLays a defeat sound.
			printFinish(f, c, renderer, finaleTextSurface, finaleTextTexture, font, colorTip, false);
			free(f);
			f = NULL;
			free(c);
			c = NULL;
			stage_is_running = false;
		}			

		SDL_RenderPresent(renderer);
	}	
}

// Closes window, free memory from stuff and terminate process.
void destroy_window()
{
	TTF_CloseFont(font);
	Mix_FreeMusic(backgroundMusicMenu);
	Mix_FreeMusic(backgroundMusicStage);
	Mix_FreeChunk(soundEffectL);
	Mix_FreeChunk(soundEffectR);
	Mix_FreeChunk(soundEffectMenu);
	Mix_FreeChunk(soundEffectMine);
	Mix_FreeChunk(soundEffectVictory);
	Mix_Quit();
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
		if(stage_is_running)
			setup_stage(w, h, m); // Initializes minefield parameters.

		while (stage_is_running)
		{
			process_input(); // Process user mouse/keyboard inputs in stage.
			update(); // Process game objects states in stage.
			render(); // Process object rendering in stage.
			wait_interval(); // When a game is finished, waits 3 seconds ans stops music before going back to the main menu.
		}		
	}
	
	destroy_window(); // Closes window and terminate process.
	return 0;
}