
#include "../../includes/header.h"

int is_king_in_check(t_gui *gui, int is_white_king)
{
    t_case *king_square = NULL;

    // Trouver la position du roi
    for (int i = 0; i < 64; i++)
    {
        if (is_white_king && gui->case_list[i].status == (WHITE | KING))
        {
            king_square = &gui->case_list[i];
            printf("Le roi blanc se trouve en %d ou en %d\n", i, get_square_from_xy(king_square->startX, king_square->startY));
            break;
        }
        else if (!is_white_king && gui->case_list[i].status == (BLACK | KING))
        {
            king_square = &gui->case_list[i];
            printf("Le roi noir se trouve en %d ou en %d\n", i, get_square_from_xy(king_square->startX, king_square->startY));
            break;
        }
    }

    // Vérifier si le roi est en échec
    for (int i = 0; i < 64; i++)
    {
        t_case *attacker_square = &gui->case_list[i];
        if (is_white_piece(attacker_square) != game.white_to_play) // Si la case contient une pièce de l'adversaire
        {

            if (move_is_conform(gui, attacker_square, king_square))
            {
                printf("Case attaquante %d, case du roi %d\n", get_square_from_xy(attacker_square->startX, attacker_square->startY), get_square_from_xy(king_square->startX, king_square->startY));
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
				printf("%d ", square->status);
			//printf("%d, ", square[square_n].status);
		}
		printf("\n");
	}
	
}