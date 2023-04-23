#include "../../includes/header.h"

int piece_value(t_case *piece)
{
    switch (piece->status)
    {
        case WHITE | PAWN || BLACK | PAWN:
            return 1;
        case KNIGHT:
        case BISHOP:
            return 3;
        case ROOK:
            return 5;
        case QUEEN:
            return 9;
        default:
            return 0;
    }
}


int compare_moves(const void *a, const void *b)
{
    t_move *move_a = (t_move *)a;
    t_move *move_b = (t_move *)b;

    return move_b->score - move_a->score;
}

void process_AI(t_gui *gui)
{
    t_move valid_moves[64 * 64];
    int move_count;

    if (has_valid_moves(gui, 0) == 0)
    {
        printf("White win gg\n");
        return ;
    }
    random_black_move(gui);
    /*generate_valid_moves(gui, game.white_to_play, valid_moves, &move_count);
    qsort(valid_moves, move_count, sizeof(t_move), compare_moves);

    // Jouer le coup avec le score le plus élevé
    if (move_count > 0)
    {
        printf("OK\n");
        t_case *start_square = &gui->case_list[(valid_moves[0].start_square->startX, valid_moves[0].start_square->startY)];
        t_case *end_square =  &gui->case_list[(valid_moves[0].end_square->startX, valid_moves[0].end_square->startY)];
        move_is_valid(gui, start_square, end_square);
        move_piece(gui, start_square, end_square);
    }
    else
    {
        printf("No valid moves, game over.\n");
    }*/
}

void random_black_move(t_gui *gui)
{
    int start_index, end_index;
    t_case *start_square, *end_square;
    int is_valid = 0;

    srand(time(NULL));

    if (has_valid_moves(gui, 0) == 0)
    {
        printf("White win gg\n");
        return ;
    }
    while (!is_valid)
    {
        start_index = rand() % 64;
        end_index = rand() % 64;
        start_square = &gui->case_list[start_index];
        end_square = &gui->case_list[end_index];

        if (is_white_piece(start_square) == 0 && move_is_valid(gui, start_square, end_square))
        {
            is_valid = 1;
        }
    }

    move_piece(gui, start_square, end_square);
}

void generate_valid_moves(t_gui *gui, int is_white, t_move *valid_moves, int *move_count)
{
    int start_index, end_index;
    t_case *start_square, *end_square;


    *move_count = 0;

    for (start_index = 0; start_index < 64; start_index++)
    {
        t_gui *temp_gui = clone_t_gui(gui);
        start_square = &temp_gui->case_list[start_index];

        if ((is_white_piece(start_square) && is_white) || (is_white_piece(start_square) == 0 && !is_white))
        {
            for (end_index = 0; end_index < 64; end_index++)
            {
                end_square = &temp_gui->case_list[end_index];

                if (move_is_valid(temp_gui, start_square, end_square))
                {
                    valid_moves[*move_count].start_square = start_square;
                    valid_moves[*move_count].end_square = end_square;
                    valid_moves[*move_count].score += piece_value(end_square);
                    (*move_count)++;
                }
            }
        }
    }
}

