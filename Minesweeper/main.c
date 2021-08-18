#include <stdio.h>
#include <stdlib.h>
#include "initField.h"
#include "fillField.h"
#include "printField.h"
#include "game.h"

int main()
{
	startGame(); // Starts a new game.
	while (1)
	{
		// Checks if the player wants to play again. If not, exit the program.
		char c = ' ';
		printf("\033[0;37m"); // Prints in white.
		printf("\n Deseja jogar novamente? (S ou s para confirmar, qualquer outra tecla para sair)");
		scanf_s(" %c", &c, sizeof(char));
		if (c == 'S' || c == 's')
		{
			system("cls"); // Clear the screen.
			startGame(); // Starts a new game.
		}
		else
			exit(0); // Closes game.
	}
	
}