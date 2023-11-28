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



int evaluate_board(t_gui *gui)
{
	int total_score = 0;
	for (int i = 0; i < 64; i++)
	{
		total_score += piece_value(&gui->case_list[i]);
	}
	return (total_score);
}

/*int minimax(t_game *game, int depth, int alpha, int beta, int maximizing_player)
{
    if (depth == 0 || has_valid_moves(gui, maximizing_player) == 0)
    {
        return evaluate_board(gui);
    }

    t_move valid_moves[64 * 64];
    int move_count;
    generate_valid_moves(game, maximizing_player, valid_moves, &move_count);

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
}*/

void process_AI(t_game *game) {
    t_move *valide_moves = malloc(sizeof(t_move) * 1000);
    int i = generate_valid_moves(game->bitboards, game->white_to_play, valide_moves);
    printf("Numbe of valide moves %d\n", i);
    if (i == 0) return;
    make_move_bitboards(game->bitboards, get_status_by_index(valide_moves[0].start_index, game->bitboards), valide_moves[0].start_index, valide_moves[0].end_index);
    move_piece(gui, valide_moves[0].start_square, valide_moves[0].end_square);
    /*t_move valid_moves[64 * 64];
    t_game *temp_game = clone_t_game(game);
    int move_count = generate_valid_moves(temp_game, 0, valid_moves);

    if (move_count == 0) {
        printf("No valid moves, game over\n");
        return;
    }

    int max_eval = INT_MIN;
    t_move best_move;

    for (int i = 0; i < move_count; i++) {
        temp_game = clone_t_game(game); // Clone the game state including bitboards
        apply_move_to_bitboards(temp_game->bitboards, valid_moves[i].start_index, valid_moves[i].end_index);
        int eval = minimax(temp_game, 1, INT_MIN, INT_MAX, 1); // Adjust depth as necessary
        if (eval > max_eval) {
            max_eval = eval;
            best_move = valid_moves[i];
        }
        free_t_game(temp_game); // Clean up the cloned game state
    }
    printf("Move piece from %d to %d\n", best_move.start_index, best_move.end_index);

    // Apply the best move found to the original game state
    apply_move_to_bitboards(game->bitboards, best_move.start_index, best_move.end_index);*/
}







/*void random_black_move(t_gui *gui)
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
}*/




