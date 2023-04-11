
#include "../../includes/header.h"

int is_king_in_check(t_gui *gui, int is_white_king)
{
    t_case *king_square = NULL;

    // Trouver la position du roi
    for (int i = 0; i < 64; i++)
    {
        if ((gui->case_list[i].status & (KING | (is_white_king ? WHITE : BLACK))) == (KING | (is_white_king ? WHITE : BLACK)))
        {
            king_square = &gui->case_list[i];
			printf("le roi se trouve en %d\n", i);
            break;
        }
    }

    // Vérifier si le roi est en échec
	print_board_in_term(gui);
    for (int i = 0; i < 64; i++)
    {
        t_case *attacker_square = &gui->case_list[i];
        if ((attacker_square->status & (is_white_king ? BLACK : WHITE)) != 0) // Si la case contient une pièce de l'adversaire
        {
            if (move_is_conform(gui, attacker_square, king_square))
            {
                return 1;
            }
        }
    }

    return 0;
}

void print_board_in_term(t_gui *gui)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int square_n = i * 8 + j;
			t_case *square = &gui->case_list[square_n];
			if (square->status == 0)
			{
				printf("- ");
			}
			else
				printf("x ");
			//printf("%d, ", square[square_n].status);
		}
		printf("\n");
	}
	
}