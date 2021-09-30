#pragma once
#ifndef _OPENSPACEFIELD_H_
#define _OPENSPACEFIELD_H_
#include "initField.h"

void openFieldUtil(field_t *f, field_t *c, int x, int y, int old);

void openField(field_t *f, field_t *c, int x, int y, int flag, int *flagQty);

#endif