//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <SDL.h>
//#include "game.h"
//#include "initField.h"
//#include "fillField.h"
//#include "sumMineField.h"
//#include "printField.h"
//#include "openSpaceField.h"
//
// Checks if the player beat the game.
//bool checkWin(field_t *f, field_t *c)
//{
//	 If each upper slot equals cover AND lower slot equals mine, OR, upper slot equals flag AND lower slot equals mine, OR, upper slot equals lower slot, CONTINUE. If not, player still not won.
//	for (int i = 1; i < c->x - 1; i++)
//		for (int j = 1; j < c->y - 1; j++)
//			if ((c->mat[i][j] == COVER && f->mat[i][j] == MINE) || (c->mat[i][j] == FLAG && f->mat[i][j] == MINE) || (c->mat[i][j] == f->mat[i][j]))
//				continue;
//			else
//				return false;
//	return true;
//}
//
// Checks if the player lost the game.
//bool checkLose(field_t *f, field_t *c, int *inpt)
//{
//	 If lower slot equals mine AND upper slot not equals flag AND player choose 'open' as a move, player LOST.
//	if (f->mat[inpt[0]][inpt[1]] == MINE && c->mat[inpt[0]][inpt[1]] != FLAG && inpt[2] == 1)
//		return true;
//	else
//		return false;
//}
//
// Prints a victory line.
//void printWin(field_t *f, field_t *c, bool fnsh)
//{
//	if (fnsh)
//	{
//		 Prints flags on every mine when player wins.
//		for(int i = 1; i < c->x - 1; i++)
//			for(int j = 1; j < c->y - 1; j++)
//				if(f->mat[i][j] == MINE)
//					c->mat[i][j] = FLAG;
//		system("cls");
//		printField(c); // Prints upper field to the player.
//		printf("\033[0;34m"); // Prints in blue.
//		printf("\n Voce venceu!");
//		free(f);
//		free(c);
//	}
//}
//
// Prints a defeat line.
//void printLose(field_t *f, field_t *c)
//{
//	system("cls");
//	printField(c); // Prints upper field to the player.
//	printf("\033[0;31m"); // Prints in red.
//	printf("\n Voce perdeu!");
//	free(f);
//	free(c);
//}
//
// Prints title with version.
//void printTitle()
//{
//	printf("\033[0;37m"); // Prints in white.
//	printf("\n Campo Minado - v1.0 - Implementado por Kevin Costa.\n\n");
//}
//
// Verify player's input and returns to an array, before placing the mines, so the player won't die right away.
//int *verifyInput(field_t* f)
//{
//	int x, y, flag;
//	int *vec = (int*)malloc(3 * sizeof(int)); // Allocates the array which will be used to store the return values.
//
//	 Asks for the line and column that the player wants to open/flag, within specified limits.
//	do
//	{
//		//printf("\033[0;37m"); // Prints in white.
//		//printf("\n Qual linha deseja selecionar? ");
//		//scanf_s("%d", &x);
//		//printf("\n Qual coluna deseja selecionar? ");
//		//scanf_s("%d", &y);
//		if (x >= f->x - 1 || y >= f->y - 1 || x < 1 || y < 1)
//		{
//			//printf("\033[0;31m"); // Prints in red.
//			//printf(" Campo escolhido esta fora da area permitida.");
//		}
//	} while (x >= f->x - 1 || y >= f->y - 1 || x < 1 || y < 1);
//	
//	// Asks if the player wants to open or flag.
//	do
//	{
//		/*printf("\n Abrir(1) ou bandeira(2)? ");
//		scanf_s("%d", &flag);*/
//	} while (flag != 1 && flag != 2);
//		
//	/*vec[0] = x;
//	vec[1] = y;
//	vec[2] = flag;*/
//	
//	return vec;
//}
//
// Starts a new game
//void startGame()
//{
//	int *inpt = (int*)malloc(3 * sizeof(int)); // Allocates the array which will be used to store args used later.
//	int w, h, m;
//	bool finish = true, lose = false;
//	field_t *f, *c;
//
//	printf("\033[0;37m"); // Prints in white.
//	printTitle(); // Prints title with version.
//
//	 Asks for width and height of the field, within specified limits.
//	do
//	{		
//		/*printf("\033[0;37m");
//		printf(" Qual a largura do campo? (Minimo 4, Maximo 30) ");
//		scanf_s("%d", &w);
//		printf(" Qual a altura do campo? (Minimo 4, Maximo 30) ");
//		scanf_s("%d", &h);*/
//		if (w < 4 || w > 30 || h < 4 || h > 30)
//		{
//			/*printf("\033[0;31m");
//			printf(" Tamanho esta fora do permitido.\n");*/
//		}
//	} while (w < 4 || w > 30 || h < 4 || h > 30);
//	
//	// Asks for the amount of mines, greater than zero and less than the total field matrix slots.
//	do
//	{
//		/*printf("\033[0;37m");
//		printf(" Quantas minas? ");
//		scanf_s("%d", &m);*/
//		if (w * h <= m)
//		{
//			/*printf("\033[0;31m");
//			printf(" Numero de minas maior do que o campo minado suporta.\n");*/
//		}
//		if (m < 1)
//		{
//			/*printf("\033[0;31m");
//			printf(" Numero de minas deve ser maior que zero.\n");*/
//		}
//	} while (w * h <= m || m < 1);
//
//	f = initField(w, h, m); // Allocates the lower field, where mines and tips will be written.
//	c = initCover(w, h); // Allocates the upper field, where the player will uncover.
//	fillFieldEdge(c); // Fills the upper field with edge characters.
//	fillFieldCover(c); // Fills the upper field with cover characters.
//	//system("cls"); // Clear the screen.
//	//printField(c); // Prints upper field to the player.
//	inpt = verifyInput(f); // Verify player's input and returns to an array, before placing the mines, so the player won't die right away.
//	fillFieldZero(f); // Fills the lower field with zeros.
//	fillFieldEdge(f); // Fills the lower field with edge characters.
//	fillFieldMine(f, inpt); // Fills the lower field with mines, according to the array previously created.
//	countMines(f); // Calculates the amount of mines and fills tips on mines's neighborhoods.
//	openField(f, c, inpt[0], inpt[1], inpt[2]); // Opens slot and every empty (zero) slot adjacent to it (flood fill).
//
//	finish = checkWin(f, c); // Checks if the player beat the game.
//	if (finish)
//	{
//		printWin(f, c, finish); // Prints a victory line.
//	}
//	else
//	{
//		while (1)
//		{
//			// After the first move, every move will be placed inside this loop.
//			system("cls"); // Clear the screen.
//			printField(c); // Prints upper field to the player.
//			inpt = verifyInput(f); // Verify player's input and returns to an array, before placing the mines, so the player won't die right away.
//			openField(f, c, inpt[0], inpt[1], inpt[2]); // Opens slot and every empty (zero) slot adjacent to it (flood fill).
//
//			lose = checkLose(f, c, inpt); // Checks if the player lost the game.
//			if (lose)
//			{
//				printLose(f, c); // Prints a defeat line.
//				break;
//			}
//
//			finish = checkWin(f, c); // Checks if the player beat the game.
//			if (finish)
//			{
//				printWin(f, c, finish); // Prints a victory line.
//				break;
//			}
//		}
//	}	
//}