#include <stdio.h>
#include "sumMineField.h"
#include "fillField.h"

// Calculates the amount of mines and fills tips on mines's neighborhoods.
void countMines(field_t *f)
{
	for (int i = 0; i < f->x; i++)
		for (int j = 0; j < f->y; j++)
		{
			// If lower slot is not edges and mines, increases every mine adjacent slot counter.
			if (f->mat[i][j] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i][j] != MINE)
			{
				if (f->mat[i][j + 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i][j + 1] == MINE)
				{
					f->mat[i][j]++;
				}
				if (f->mat[i][j - 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i][j - 1] == MINE)
				{
					f->mat[i][j]++;
				}
				if (f->mat[i + 1][j] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i + 1][j] == MINE)
				{
					f->mat[i][j]++;
				}
				if (f->mat[i - 1][j] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i - 1][j] == MINE)
				{
					f->mat[i][j]++;
				}
				if (f->mat[i + 1][j + 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i + 1][j + 1] == MINE)
				{
					f->mat[i][j]++;
				}
				if (f->mat[i - 1][j - 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i - 1][j - 1] == MINE)
				{
					f->mat[i][j]++;
				}
				if (f->mat[i - 1][j + 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i - 1][j + 1] == MINE)
				{
					f->mat[i][j]++;
				}
				if (f->mat[i + 1][j - 1] != EDGE_T_B && f->mat[i][j] != EDGE_L_R && f->mat[i + 1][j - 1] == MINE)
				{
					f->mat[i][j]++;
				}
			}			
		}
}