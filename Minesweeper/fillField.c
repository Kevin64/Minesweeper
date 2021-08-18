#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "fillField.h"
#include "initField.h"

// Fills field with zeros.
void fillFieldZero(field_t *f)
{
	for (int i = 0; i < f->x; i++)
		for (int j = 0; j < f->y; j++)
			f->mat[i][j] = BLANK;
}

// Fills field with edge characters.
void fillFieldEdge(field_t *f)
{
	for (int i = 0; i < f->x; i++)
	{
		f->mat[i][0] = EDGE_T_B;		
		f->mat[i][f->y - 1] = EDGE_T_B;		
	}
	for (int j = 0; j < f->y; j++)
	{
		f->mat[0][j] = EDGE_L_R;
		f->mat[f->x - 1][j] = EDGE_L_R;
	}
}

// Fills field with mines.
void fillFieldMine(field_t *f, int *a)
{
	srand(time(NULL));
	for (int i = 0; i < f->m; i++)
	{
		int x = rand() % f->x; // Randomly places mines in x.
		int y = rand() % f->y; // Randomly places mines in y.

		// Places mines where there are not edges, existing mines, and where the player made the first move.
		if (f->mat[x][y] != MINE && f->mat[x][y] != EDGE_T_B && f->mat[x][y] != EDGE_L_R && a[0] != x && a[1] != y)
			f->mat[x][y] = MINE;
		else
			i--;
	}
}

// Fills field with cover characters.
void fillFieldCover(field_t* f)
{
	for (int i = 1; i < f->x - 1; i++)
		for (int j = 1; j < f->y - 1; j++)
			f->mat[i][j] = COVER;	
}