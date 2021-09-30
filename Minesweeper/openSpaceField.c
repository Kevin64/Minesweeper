#include <stdio.h>
#include <stdbool.h>
#include "openSpaceField.h"
#include "fillField.h"

// Auxiliary function for flood filling.
 void openFieldUtil(field_t *f, field_t *c, int x, int y, int old)
{
	 if (x < 0 || x >= c->x || y < 0 || y >= c->y)
		 return;
	 if (c->mat[x][y] == FLAG)
		 return;
	 if (c->mat[x][y] != old)
		 return;
	 if (c->mat[x][y] == f->mat[x][y])
		 return;
	 if (f->mat[x][y] == MINE)
	 {
		 c->mat[x][y] = f->mat[x][y];
		 return;
	 }		 
	 if (f->mat[x][y] != 0)
	 {
		 c->mat[x][y] = f->mat[x][y];
		 return;
	 }

	 c->mat[x][y] = f->mat[x][y];

	 // Recursion for flood filling.
	 openFieldUtil(f, c, x + 1, y, old);
	 openFieldUtil(f, c, x - 1, y, old);
	 openFieldUtil(f, c, x, y + 1, old);
	 openFieldUtil(f, c, x, y - 1, old);
}

 // Opens slot and every empty (zero) slot adjacent to it (flood fill).
 void openField(field_t *f, field_t* c, int x, int y, int flag, int *flagQty)
 {
	 // If player opens and upper slot equals cover.
	 if (flag == 1 && c->mat[x][y] == COVER)
	 {
		 int old = c->mat[x][y];
		 if (old == f->mat[x][y])
			 return;
		 openFieldUtil(f, c, x, y, old);
	 }
	 // Else, if player flags upper slot and it's not flagged, flags.
	 else if (flag == 2 && c->mat[x][y] != FLAG && c->mat[x][y] != f->mat[x][y])
	 {
		 c->mat[x][y] = FLAG;
		 (*flagQty)--;
	 }
	 // Else, if player flags upper slot and it's not cover AND it's not flagged AND it's not mine, do nothing.
	 else if (flag == 2 && c->mat[x][y] != COVER && c->mat[x][y] != FLAG && c->mat[x][y] != MINE)
		 c->mat[x][y] = c->mat[x][y];
	 // Else, if player flags upper slot and it's already flagged, covers again.
	 else if (flag == 2 && c->mat[x][y] == FLAG)
	 {
		 c->mat[x][y] = COVER;
		 (*flagQty)++;
	 }
	 else if (flag == 2 && c->mat[x][y] == f->mat[x][y]);
 }