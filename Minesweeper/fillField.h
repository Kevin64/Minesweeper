#pragma once
#ifndef _FILLFIELD_H_
#define _FILLFIELD_H_
#include "initField.h"

#define OPEN_F 1
#define FLAG_F 2
#define MINE 20
#define BLANK 0
#define COVER 10
#define FLAG 50
#define EDGE_T_B 100
#define EDGE_L_R 101

void fillFieldZero(field_t *f);

void fillFieldEdge(field_t *f);

void fillFieldMine(field_t *f);

void fillFieldCover(field_t* f);

#endif