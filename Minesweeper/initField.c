#include <stdio.h>
#include <stdlib.h>
#include "initField.h"

// Allocates the lower field, where mines and tips will be written.
field_t *initField(int x, int y, int mines)
{
	field_t *fieldShow = (field_t*)malloc(sizeof(field_t));
	fieldShow->x = x + 2;
	fieldShow->y = y + 2;
	fieldShow->m = mines;
	fieldShow->mat = (int**)malloc((fieldShow->x) * sizeof(int*));
	for (int i = 0; i < fieldShow->x; i++)
		fieldShow->mat[i] = (int*)malloc((fieldShow->y) * sizeof(int));
	return fieldShow;
}

// Allocates the upper field, where the player will uncover.
field_t *initCover(int x, int y)
{
	field_t *fieldHide = (field_t*)malloc(sizeof(field_t));
	fieldHide->x = x + 2;
	fieldHide->y = y + 2;
	fieldHide->m = 0;
	fieldHide->mat = (int**)malloc((fieldHide->x) * sizeof(int*));
	for (int i = 0; i < fieldHide->x; i++)
		fieldHide->mat[i] = (int*)malloc((fieldHide->y) * sizeof(int));
	return fieldHide;
}