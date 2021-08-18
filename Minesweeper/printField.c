#include <stdio.h>
#include "printField.h"
#include "initField.h"
#include "fillField.h"
#include "game.h"

// Prints upper field to the player.
void printField(field_t *f)
{
	printTitle(); // Prints title with version.
	for (int i = 0; i < f->x; i++)
	{
		printf("\n");
		for (int j = 0; j < f->y; j++)
		{
			if (f->mat[i][j] == MINE)
			{
				printf("\033[0;31m"); // Prints in red.
				printf(" M "); // Prints a mine.
			}
			else if (f->mat[i][j] == EDGE_T_B)
			{
				printf("\033[0;36m"); // Prints in cyan.
				printf(" | "); // Prints left and right edges.
			}
			else if (f->mat[i][j] == EDGE_L_R)
			{
				printf("\033[0;36m"); // Prints in cyan.
				printf(" - "); // Prints top and bottom edges.
			}
			else if (f->mat[i][j] == COVER)
			{
				printf("\033[0;37m"); // Prints in white.
				printf(" ? "); // Prints a cover.
			}
			else if (f->mat[i][j] == FLAG)
			{
				printf("\033[0;33m"); // Prints in yellow.
				printf(" X "); // Prints a flag.
			}
			else
			{
				printf("\033[0;32m"); // Prints in green.
				printf(" %d ", f->mat[i][j]); // Prints a tip/zero.
			}
		}
	}
}