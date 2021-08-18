#pragma once
#ifndef _GAME_H_
#define _GAME_H_
#include <stdbool.h>
#include "initField.h"

bool checkWin(field_t* f, field_t* c);

bool checkLose(field_t* f, field_t* c, int* inpt);

void printWin(field_t* f, field_t* c, bool fnsh);

void printLose(field_t* f, field_t* c);

void printTitle();

int *verifyInput(field_t* f);

void startGame();

#endif