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
Mix_Music *backgroundMusicMenu,
			*backgroundMusicStage;
Mix_Chunk *soundEffectL,
			*soundEffectR,
			*soundEffectMine,
			*soundEffectMenu,
			*soundEffectVictory;
SDL_Surface //*windowIconSurface,
			*titleTextSurface,
			*bgScrollSurface,
			*menuPresentationSurface,
			*menuTextSurface1,
			*menuTextSurface2,
			*labelSurface,
			*textInputSurface,
			*okButtonSurface,
			*alertTextSurface,	
			*tileTextSurface,
			*mineIconSurface,
			*mineBoomIconSurface,
			*mineDeathIconSurface,
			*flagIconSurface,
			*edgeIconSurface,
			*coverIconSurface,
			*finaleTextSurface;
SDL_Texture //*windowIconTexture,
			*titleTextTexture,
			*bgScrollTexture,	
			*menuPresentationTexture,
			*menuTextTexture1,
			*menuTextTexture2,
			*labelTexture,
			*textInputTexture,
			*okButtonTexture,
			*alertTextTexture,	
			*tileTextTexture,
			*mineIconTexture,
			*mineBoomIconTexture,
			*mineDeathIconTexture,
			*flagIconTexture,
			*edgeIconTexture,
			*coverIconTexture,
			*finaleTextTexture;
SDL_Rect bgScrollRect1,
		bgScrollRect2,
		menuPresentationRect,
		menuButtonRect1,
		menuButtonRect2,
		widthFieldLabelRect,
		widthFieldTextboxRect,
		heightFieldLabelRect,
		heightFieldTextboxRect,
		mineAmountLabelRect,
		mineAmountTextboxRect,		
		okButtonRect,
		tileSquareRect;

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
bool soundEffectPlayed = false;
int last_frame_time = 0;
int length, option = 0, formField = 0;
int alpha1 = ALPHA_UNSELECTED, alpha2 = ALPHA_UNSELECTED, alpha3 = ALPHA_UNSELECTED, alpha4 = ALPHA_UNSELECTED;
int i, j, counter1 = 1, counter2 = 1, counter3 = 1;
int ij_selected[3];
int xm, ym, xi, xf, yi, yf;
int button_x, button_y, button_w, button_h;
int w, h, m;
int centerFieldX, centerFieldY, centerFormTextX, centerFormInputX;
int angle = 0;
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
	//windowIconSurface = IMG_Load(WINDOW_ICON);
	//SDL_SetWindowIcon(window, windowIconSurface);

	// Load support for PNG image formats
	int flags = IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		fprintf(stderr, ERROR_SDL_INIT_PNG);
		return false;
	}

	// Sets icons for assets.
	bgScrollSurface = IMG_Load(BACKGROUND_WALLPAPER);
	menuPresentationSurface = IMG_Load(PRESENTATION);
	menuTextSurface1 = IMG_Load(NEW_OK_BUTTON);
	menuTextSurface2 = IMG_Load(QUIT_BUTTON);
	okButtonSurface = IMG_Load(NEW_OK_BUTTON);
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

	// Initializes background animation coordinates.
	bgScrollRect1.x = 0;
	bgScrollRect1.y = 0;
	bgScrollRect1.w = WINDOW_WIDTH;
	bgScrollRect1.h = WINDOW_HEIGHT;
	bgScrollRect2.x = -WINDOW_WIDTH;
	bgScrollRect2.y = 0;
	bgScrollRect2.w = WINDOW_WIDTH;
	bgScrollRect2.h = WINDOW_HEIGHT;	

	// Returns true if all is ok.
	return true;
}

// Initializes menu parameters.
void setup_main_menu()
{	
	// Sets presentation image size.
	menuPresentationRect.w = 256;
	menuPresentationRect.x = WINDOW_WIDTH / 2 - menuPresentationRect.w / 2;
	menuPresentationRect.h = 256;
	menuPresentationRect.y = WINDOW_HEIGHT / 4 - menuPresentationRect.h / 2;

	// Sets button size.
	button_x = BUTTON_X;
	button_y = BUTTON_Y;
	button_w = BUTTON_W;
	button_h = BUTTON_H;

	// New game button position.
	menuButtonRect1.x = button_x + ((WINDOW_WIDTH / 2) - button_w / 2);
	menuButtonRect1.y = button_y + ((WINDOW_HEIGHT / 2) - button_h / 3);
	menuButtonRect1.w = button_w;
	menuButtonRect1.h = button_h;
	
	// Quit game button position.
	menuButtonRect2.x = menuButtonRect1.x;
	menuButtonRect2.y = menuButtonRect1.y + BUTTON_SPACING;
	menuButtonRect2.w = button_w;
	menuButtonRect2.h = button_h;
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
						SDL_SetTextInputRect(&widthFieldLabelRect);
						strcat(paramInput1, event.text.text);
						counter1 += TEXT_BOX_FINE_ADJUSTEMENT;
					}
					else if (paramInput2[1] == '\0' && formField == 1)
					{
						SDL_SetTextInputRect(&widthFieldLabelRect);
						strcat(paramInput2, event.text.text);
						counter2 += TEXT_BOX_FINE_ADJUSTEMENT;
					}
					else if (paramInput3[2] == '\0' && formField == 2)
					{
						SDL_SetTextInputRect(&widthFieldLabelRect);
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

	// If background wallpaper left size reaches the rightmost side, resets to the start.
	if (bgScrollRect1.x > WINDOW_WIDTH)
		bgScrollRect1.x = 0;
	if(bgScrollRect2.x > 0)
		bgScrollRect2.x = -WINDOW_WIDTH;
	bgScrollRect1.x += 4; // Slides wallpaper 1 to the right.
	bgScrollRect2.x += 4; // Slides wallpaper 2 to the right.
	
	// Changes the angle of presentation.
	if (angle > 360)
		angle = 0;
	else
		angle++;
}

// Process object rendering in game.
void render()
{
	// If main menu is running.
	if (main_menu_is_running && !select_menu_is_running && !stage_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Background animation.
		bgScrollTexture = SDL_CreateTextureFromSurface(renderer, bgScrollSurface);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);
		SDL_DestroyTexture(bgScrollTexture);
		
		// Presentation image.
		menuPresentationTexture = SDL_CreateTextureFromSurface(renderer, menuPresentationSurface);
		SDL_RenderCopyEx(renderer, menuPresentationTexture, NULL, &menuPresentationRect, angle, NULL, SDL_FLIP_NONE);
		SDL_DestroyTexture(menuPresentationTexture);

		// Menu buttons.
		menuTextTexture1 = SDL_CreateTextureFromSurface(renderer, menuTextSurface1);
		menuTextTexture2 = SDL_CreateTextureFromSurface(renderer, menuTextSurface2);
		SDL_RenderCopy(renderer, menuTextTexture1, NULL, &menuButtonRect1);
		SDL_RenderCopy(renderer, menuTextTexture2, NULL, &menuButtonRect2);
		SDL_DestroyTexture(menuTextTexture1);
		SDL_DestroyTexture(menuTextTexture2);

		// Prints title.
		printTitle(renderer, titleTextSurface, titleTextTexture, font, colorTitle);

		// If mouse is over new game button, highlights itself and if clicked, go to select size/mines screen.
		if (xm >= menuButtonRect1.x && xm <= menuButtonRect1.x + menuButtonRect1.w && ym >= menuButtonRect1.y && ym <= menuButtonRect1.y + menuButtonRect1.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the button area.
			if (!soundEffectPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
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
		else if (xm >= menuButtonRect2.x && xm <= menuButtonRect2.x + menuButtonRect2.w && ym >= menuButtonRect2.y && ym <= menuButtonRect2.y + menuButtonRect2.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the button area.
			if (!soundEffectPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
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
			soundEffectPlayed = false; // If the mouse exits any button area, sets sound effect to play again on hover.
		
		// Highlisghts new game button.
		if (option == 0)
		{
			printFormText(renderer, menuTextSurface1, menuTextTexture1, font, colorMenuText, menuButtonRect1, NEW_GAME_TEXT, 0, 0, 0, 0);
			printFormText(renderer, menuTextSurface2, menuTextTexture2, font, colorMenuText, menuButtonRect2, QUIT_GAME_TEXT, 0, 0, 0, 127);
		}
		// Highlisghts quit game button.
		else if (option == 1)
		{
			printFormText(renderer, menuTextSurface2, menuTextTexture2, font, colorMenuText, menuButtonRect1, NEW_GAME_TEXT, 0, 0, 0, 127);
			printFormText(renderer, menuTextSurface2, menuTextTexture2, font, colorMenuText, menuButtonRect2, QUIT_GAME_TEXT, 0, 0, 0, 0);
		}	

		SDL_RenderPresent(renderer);
	}
	// If select menu is running.
	else if (!main_menu_is_running && select_menu_is_running && !stage_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		// Background animation.
		bgScrollTexture = SDL_CreateTextureFromSurface(renderer, bgScrollSurface);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);
		SDL_DestroyTexture(bgScrollTexture);

		// Prints title.
		printTitle(renderer, titleTextSurface, titleTextTexture, font, colorTitle);

		widthFieldLabelRect.y = WIDTH_TEXT_Y;
		widthFieldLabelRect.w = WIDTH_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (widthFieldLabelRect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		widthFieldLabelRect.h = WIDTH_TEXT_H;
		widthFieldLabelRect.x = centerFormTextX;
		
		widthFieldTextboxRect.y = WIDTH_INPUT_Y;
		widthFieldTextboxRect.w = WIDTH_INPUT_W + counter1;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (widthFieldTextboxRect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		widthFieldTextboxRect.h = WIDTH_INPUT_H;
		widthFieldTextboxRect.x = centerFormInputX;
		printFormText(renderer, textInputSurface, textInputTexture, font, colorForm, widthFieldTextboxRect, paramInput1, 0, 0, 0, 255);

		heightFieldLabelRect.y = HEIGHT_TEXT_Y;
		heightFieldLabelRect.w = HEIGHT_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (heightFieldLabelRect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		heightFieldLabelRect.h = HEIGHT_TEXT_H;
		heightFieldLabelRect.x = centerFormTextX;
		
		heightFieldTextboxRect.y = HEIGHT_INPUT_Y;
		heightFieldTextboxRect.w = HEIGHT_INPUT_W + counter2;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (heightFieldTextboxRect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		heightFieldTextboxRect.h = HEIGHT_INPUT_H;
		heightFieldTextboxRect.x = centerFormInputX;
		printFormText(renderer, textInputSurface, textInputTexture, font, colorForm, heightFieldTextboxRect, paramInput2, 0, 0, 0, 255);

		mineAmountLabelRect.y = MINE_TEXT_Y;
		mineAmountLabelRect.w = MINE_TEXT_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (mineAmountLabelRect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		mineAmountLabelRect.h = MINE_TEXT_H;
		mineAmountLabelRect.x = centerFormTextX;

		mineAmountTextboxRect.y = MINE_INPUT_Y;
		mineAmountTextboxRect.w = MINE_INPUT_W + counter3;
		centerFormInputX = ((WINDOW_WIDTH / 2) - (mineAmountTextboxRect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		mineAmountTextboxRect.h = MINE_INPUT_H;
		mineAmountTextboxRect.x = centerFormInputX;
		printFormText(renderer, textInputSurface, textInputTexture, font, colorForm, mineAmountTextboxRect, paramInput3, 0, 0, 0, 255);

		okButtonRect.y = OK_BUTTON_Y;
		okButtonRect.w = OK_BUTTON_W;
		centerFormTextX = ((WINDOW_WIDTH / 2) - (okButtonRect.w) / 2); // Centralizes the text/form/button at the center of the window width.
		okButtonRect.h = OK_BUTTON_H;
		okButtonRect.x = centerFormTextX;

		// Draws each text label.
		printFormText(renderer, labelSurface, labelTexture, font, colorForm, widthFieldLabelRect, WIDTH_TEXT, 0, 0, 0, 255);
		printFormText(renderer, labelSurface, labelTexture, font, colorForm, heightFieldLabelRect, HEIGHT_TEXT, 0, 0, 0, 255);
		printFormText(renderer, labelSurface, labelTexture, font, colorForm, mineAmountLabelRect, MINE_AMOUNT_TEXT, 0, 0, 0, 255);

		// Draws OK button
		okButtonTexture = SDL_CreateTextureFromSurface(renderer, okButtonSurface);
		SDL_RenderCopy(renderer, okButtonTexture, NULL, &okButtonRect);
		printFormText(renderer, okButtonSurface, okButtonTexture, font, colorButton, okButtonRect, OK_BUTTON_TEXT, 0, 0, 0, alpha4);
		SDL_DestroyTexture(okButtonTexture);

		// Draws each textbox for input.
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha1);
		SDL_RenderFillRect(renderer, &widthFieldTextboxRect);
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha2);
		SDL_RenderFillRect(renderer, &heightFieldTextboxRect);
		SDL_SetRenderDrawColor(renderer, 127, 127, 127, alpha3);
		SDL_RenderFillRect(renderer, &mineAmountTextboxRect);


		// If the mouse is over the width textbox, and a click is detected, selects that box for input.
		if (xm >= widthFieldTextboxRect.x && xm <= widthFieldTextboxRect.x + widthFieldTextboxRect.w && ym >= widthFieldTextboxRect.y && ym <= widthFieldTextboxRect.y + widthFieldTextboxRect.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the textbox area.
			if (!soundEffectPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL)
				formField = 0;
		}
		// If the mouse is over the height textbox, and a click is detected, selects that box for input.
		else if (xm >= heightFieldTextboxRect.x && xm <= heightFieldTextboxRect.x + heightFieldTextboxRect.w && ym >= heightFieldTextboxRect.y && ym <= heightFieldTextboxRect.y + heightFieldTextboxRect.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the textbox area.
			if (!soundEffectPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL)
				formField = 1;
		}
		// If the mouse is over the mine textbox, and a click is detected, selects that box for input.
		else if (xm >= mineAmountTextboxRect.x && xm <= mineAmountTextboxRect.x + mineAmountTextboxRect.w && ym >= mineAmountTextboxRect.y && ym <= mineAmountTextboxRect.y + mineAmountTextboxRect.h)
		{
			// If sound effect has not been played yet, plays once if the mouse stays inside the textbox area.
			if (!soundEffectPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL)
				formField = 2;
		}
		// If the mouse is over the ok button, and a click is detected, starts a new stage.
		else if (xm >= okButtonRect.x && xm <= okButtonRect.x + okButtonRect.w && ym >= okButtonRect.y && ym <= okButtonRect.y + okButtonRect.h)
		{
			alpha4 = 0; // Highlights OK button.
			// If sound effect has not been played yet, plays once if the mouse stays inside the button area.
			if (!soundEffectPlayed)
			{
				Mix_PlayChannel(-1, soundEffectMenu, 0);
				soundEffectPlayed = true;
			}
			if (clickedL)
				formField = 3;
		}
		else
		{
			alpha4 = 127; // Darkens OK button if mouse os not over it.
			soundEffectPlayed = false; // If the mouse exits any textbox/button area, sets sound effect to play again on collision.
		}
		
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
				printAlert(renderer, alertTextSurface, alertTextTexture, font, colorAlert); // If field parameters are out of bounds, shows a banner alert.
		}

		SDL_RenderPresent(renderer);
	}
	// If game is running.
	else if (!main_menu_is_running && !select_menu_is_running && stage_is_running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		// Background animation.
		bgScrollTexture = SDL_CreateTextureFromSurface(renderer, bgScrollSurface);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect1);
		SDL_RenderCopy(renderer, bgScrollTexture, NULL, &bgScrollRect2);
		SDL_DestroyTexture(bgScrollTexture);

		// Prints title.
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

				tileSquareRect.x = xi;
				tileSquareRect.y = yi;
				tileSquareRect.w = tile.w;
				tileSquareRect.h = tile.h;

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
					SDL_RenderFillRect(renderer, &tileSquareRect);
					f->mat[i][j] = MINE_TRIG;

					// Shows a custom icon for mines.					
					mineDeathIconTexture = SDL_CreateTextureFromSurface(renderer, mineDeathIconSurface);
					SDL_RenderCopy(renderer, mineDeathIconTexture, NULL, &tileSquareRect);
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
					SDL_RenderFillRect(renderer, &tileSquareRect);

					// Shows a custom icon for edges.					
					edgeIconTexture = SDL_CreateTextureFromSurface(renderer, edgeIconSurface);
					SDL_RenderCopy(renderer, edgeIconTexture, NULL, &tileSquareRect);
					SDL_DestroyTexture(edgeIconTexture);
				}
				else if (c->mat[i][j] == FLAG)
				{
					SDL_SetRenderDrawColor(renderer, 127, 255, 0, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);

					// Shows a custom icon for flags.					
					flagIconTexture = SDL_CreateTextureFromSurface(renderer, flagIconSurface);
					SDL_RenderCopy(renderer, flagIconTexture, NULL, &tileSquareRect);
					SDL_DestroyTexture(flagIconTexture);
				}
				else if (c->mat[i][j] == COVER)
				{
					SDL_SetRenderDrawColor(renderer, 64, 64, 128, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);

					// Shows a custom icon for cover.					
					coverIconTexture = SDL_CreateTextureFromSurface(renderer, coverIconSurface);
					SDL_RenderCopy(renderer, coverIconTexture, NULL, &tileSquareRect);
					SDL_DestroyTexture(coverIconTexture);
				}
				else if (c->mat[i][j] == 0)
				{
					SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);
				}
				else
				{
					length = snprintf(NULL, 0, "%d", c->mat[i][j]);
					aux = malloc(length + 1);
					snprintf(aux, length + 1, "%d", c->mat[i][j]);

					SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);
					SDL_RenderFillRect(renderer, &tileSquareRect);

					tileTextSurface = TTF_RenderText_Blended(font, aux, colorTip);
					tileTextTexture = SDL_CreateTextureFromSurface(renderer, tileTextSurface);
					SDL_RenderCopy(renderer, tileTextTexture, NULL, &tileSquareRect);
					SDL_FreeSurface(tileTextSurface);
					SDL_DestroyTexture(tileTextTexture);
				}	

				// If the player dies, show all the hidden mines.
				if (f->mat[i][j] == MINE && showMines)
				{
					// Shows a custom icon for mines.					
					mineBoomIconTexture = SDL_CreateTextureFromSurface(renderer, mineBoomIconSurface);
					SDL_RenderCopy(renderer, mineBoomIconTexture, NULL, &tileSquareRect);
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
	SDL_FreeSurface(titleTextSurface);
	SDL_FreeSurface(bgScrollSurface);
	SDL_FreeSurface(menuPresentationSurface);
	SDL_FreeSurface(menuTextSurface1);
	SDL_FreeSurface(menuTextSurface2);
	SDL_FreeSurface(labelSurface);
	SDL_FreeSurface(textInputSurface);
	SDL_FreeSurface(okButtonSurface);
	SDL_FreeSurface(alertTextSurface);
	SDL_FreeSurface(mineIconSurface);
	SDL_FreeSurface(mineBoomIconSurface);
	SDL_FreeSurface(mineDeathIconSurface);
	SDL_FreeSurface(flagIconSurface);
	SDL_FreeSurface(edgeIconSurface);
	SDL_FreeSurface(coverIconSurface);
	SDL_FreeSurface(finaleTextSurface);

	TTF_CloseFont(font);
	TTF_Quit();

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