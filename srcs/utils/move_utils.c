
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
            break;
        }
    }

    // Vérifier si le roi est en échec
    for (int i = 0; i < 64; i++)
    {
        t_case *attacker_square = &gui->case_list[i];
        if ((attacker_square->status & (is_white_king ? BLACK : WHITE)) != 0) // Si la case contient une pièce de l'adversaire
        {
            if (move_is_valid(gui, attacker_square, king_square))
            {
                return 1;
            }
        }
    }

    return 0;
}