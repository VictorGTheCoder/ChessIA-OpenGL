#include "../../includes/header.h"

int piece_value(t_case *piece)
{
	switch (piece->status)
	{
		case WHITE | PAWN:
			return 1;
		case WHITE | BISHOP:
			return 3;
		case WHITE | ROOK:
			return 5;
		case WHITE | QUEEN:
			return 9;
		case WHITE | KING:
			return 500;

		case BLACK | PAWN:
			return -1;
		case BLACK | BISHOP:
			return -3;
		case BLACK | ROOK:
			return -5;
		case BLACK | QUEEN:
			return -9;
		case BLACK | KING:
			return -500;

		default:
			return 0;
	}
}

void generate_valid_moves(t_gui *gui, int is_white, t_move *valid_moves, int *move_count)
{
    int start_index, end_index;
    t_case *start_square, *end_square;

    t_gui *temp_gui = clone_t_gui(gui);

    *move_count = 0;

    for (start_index = 0; start_index < 64; start_index++)
    {
        start_square = &temp_gui->case_list[start_index];

        if ((is_white_piece(start_square) && is_white) || (is_white_piece(start_square) == 0 && !is_white))
        {
            for (end_index = 0; end_index < 64; end_index++)
            {
                end_square = &temp_gui->case_list[end_index];

                if (move_is_valid(temp_gui, start_square, end_square))
                {
                    valid_moves[*move_count].start_square = &temp_gui->case_list[start_index];
                    valid_moves[*move_count].end_square = &temp_gui->case_list[end_index];
                    valid_moves[*move_count].start_index = start_index;
                    valid_moves[*move_count].end_index = end_index;
                    valid_moves[*move_count].score += piece_value(end_square);
                    (*move_count)++;
                }
            }
        }
    }
}




int evaluate_board(t_gui *gui)
{
	int total_score = 0;
	for (int i = 0; i < 64; i++)
	{
		total_score += piece_value(&gui->case_list[i]);
	}
	return (total_score);
}

int minimax(t_gui *gui, int depth, int alpha, int beta, int maximizing_player)
{
    if (depth == 0 || has_valid_moves(gui, maximizing_player) == 0)
    {
        return evaluate_board(gui);
    }

    t_move valid_moves[64 * 64];
    int move_count;
    generate_valid_moves(gui, maximizing_player, valid_moves, &move_count);

    if (maximizing_player)
    {
        int max_eval = INT_MIN;
        for (int i = 0; i < move_count; i++)
        {
            t_gui *temp_gui = clone_t_gui(gui);
            move_piece(temp_gui, valid_moves[i].start_square, valid_moves[i].end_square);
            int eval = minimax(temp_gui, depth - 1, alpha, beta, 0);
            max_eval = (int)fmax(max_eval, eval);
            alpha = (int)fmax(alpha, eval);
            if (beta <= alpha)
            {
                break;
            }
        }
        return max_eval;
    }
    else
    {
        int min_eval = INT_MAX;
        for (int i = 0; i < move_count; i++)
        {
            t_gui *temp_gui = clone_t_gui(gui);
            move_piece(temp_gui, valid_moves[i].start_square, valid_moves[i].end_square);
            int eval = minimax(temp_gui, depth - 1, alpha, beta, 1);
            min_eval = fmin(min_eval, eval);
            beta = fmin(beta, eval);
            if (beta <= alpha)
            {
                break;
            }
        }
        return min_eval;
    }
}

void process_AI(t_gui *gui)
{
    t_move valid_moves[64 * 64];
    t_gui *temp_gui = clone_t_gui(gui);
    int move_count;
    generate_valid_moves(temp_gui, 0, valid_moves, &move_count);

    if (move_count == 0)
    {
        printf("No valid moves, game over\n");
        return;
    }

    int max_eval = INT_MIN;
    t_move best_move;

    for (int i = 0; i < move_count; i++)
    {
        temp_gui = clone_t_gui(gui);
        move_piece(temp_gui, valid_moves[i].start_square, valid_moves[i].end_square);
        int eval = minimax(temp_gui, 1, INT_MIN, INT_MAX, 1); // Vous pouvez ajuster la profondeur ici
        if (eval > max_eval)
        {
            max_eval = eval;
            best_move = valid_moves[i];
        }
    }
    printf("move piece\n");

    t_case *gui_start_square = &gui->case_list[best_move.start_index];
    t_case *gui_end_square = &gui->case_list[best_move.end_index];

    move_piece(gui, gui_start_square, gui_end_square);
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
