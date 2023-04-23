
#include "../../includes/header.h"

t_gui *clone_t_gui(t_gui *src)
{
    t_gui *dst = malloc(sizeof(t_gui));

    if (!dst) {
        return NULL; // Erreur d'allocation mémoire
    }

    // Copier les champs simples
    dst->img = src->img;
    dst->mlx = src->mlx;
    dst->win = src->win;
    dst->addr = src->addr;
    dst->line_length = src->line_length;
    dst->bits_per_pixel = src->bits_per_pixel;
    dst->endian = src->endian;

    // Copier les pièces
    dst->pieces = src->pieces;

    // Copier les cases
    dst->case_list = malloc(64 * sizeof(t_case));
    if (!dst->case_list) {
        free(dst);
        return NULL; // Erreur d'allocation mémoire
    }
    memcpy(dst->case_list, src->case_list, 64 * sizeof(t_case));

    // Mettre à jour les pointeurs des cases voisines
    for (int i = 0; i < 64; i++) {
        if (src->case_list[i].case_n) {
            int offset = src->case_list[i].case_n - src->case_list;
            dst->case_list[i].case_n = dst->case_list + offset;
        } else {
            dst->case_list[i].case_n = NULL;
        }

        if (src->case_list[i].case_s) {
            int offset = src->case_list[i].case_s - src->case_list;
            dst->case_list[i].case_s = dst->case_list + offset;
        } else {
            dst->case_list[i].case_s = NULL;
        }

        if (src->case_list[i].case_w) {
            int offset = src->case_list[i].case_w - src->case_list;
            dst->case_list[i].case_w = dst->case_list + offset;
        } else {
            dst->case_list[i].case_w = NULL;
        }

        if (src->case_list[i].case_e) {
            int offset = src->case_list[i].case_e - src->case_list;
            dst->case_list[i].case_e = dst->case_list + offset;
        } else {
            dst->case_list[i].case_e = NULL;
        }
    }

    // Copier le pointeur square_selected
    if (src->square_selected) {
        int offset = src->square_selected - src->case_list;
        dst->square_selected = dst->case_list + offset;
    } else {
        dst->square_selected = NULL;
    }

    return dst;
}

int has_valid_moves(t_gui *gui, int is_white)
{
    int start_index, end_index;
    t_case *start_square, *end_square;

    for (start_index = 0; start_index < 64; start_index++)
    {
        start_square = &gui->case_list[start_index];

        if ((is_white_piece(start_square) && is_white) || (is_white_piece(start_square) == 0 && !is_white))
        {
            for (end_index = 0; end_index < 64; end_index++)
            {
                end_square = &gui->case_list[end_index];

                if (move_is_valid(gui, start_square, end_square))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
