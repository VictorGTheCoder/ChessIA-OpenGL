#include "../../includes/header.h"

int get_piece_value(int status)
{
	switch (status)
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



int evaluate_board(t_bb bitboards)
{
	int total_score = 0;
	for (int i = 0; i < 64; i++)
	{
		if (get_bit(bitboards.black_pieces | bitboards.white_pieces, i) == 1)
        {
            total_score += get_piece_value(get_status_by_index(i, &bitboards));
        }
	}
	return (total_score);
}

t_payload minimax(t_game game, t_bb bitboards, int depth, int alpha, int beta, int maximizing_player)
{
    t_move bestMove;
    bestMove.end_index = 0;
    if (depth == 0)
    {
        return (t_payload){evaluate_board(bitboards), bestMove};
    }

    t_move valid_moves[1000];
    int move_count;



    move_count = generate_valid_moves(game, *game.bitboards, maximizing_player, valid_moves);

    if (maximizing_player)
    {
        int max_eval = INT_MIN;
        for (int i = 0; i < move_count; i++)
        {
            int status = get_status_by_index(valid_moves[i].start_index, &bitboards);
            make_move_bitboards(&bitboards, status, valid_moves[i].start_index, valid_moves[i].end_index);
            t_payload p = minimax(game, bitboards, depth - 1, alpha, beta, !maximizing_player);
            //make_move_bitboards(&bitboards, status, valid_moves[i].end_index, valid_moves[i].start_index);

            int eval = p.eval;
            alpha = (int)fmax(alpha, eval);

            if (max_eval < eval)
            {
                max_eval = eval;
                bestMove = valid_moves[i];
            }

            if (beta <= alpha)
                break;
        }
        printf("Actual Best Move for White: %d || From %d to %d\n", max_eval, bestMove.start_index, bestMove.end_index);

        return (t_payload){max_eval, bestMove};
    }
    else
    {
        int min_eval = INT_MAX;
        for (int i = 0; i < move_count; i++)
        {
            int status = get_status_by_index(valid_moves[i].start_index, &bitboards);
            make_move_bitboards(&bitboards, status, valid_moves[i].start_index, valid_moves[i].end_index);    
            t_payload p = minimax(game, bitboards, depth - 1, alpha, beta, !maximizing_player);
            //make_move_bitboards(&bitboards, status, valid_moves[i].end_index, valid_moves[i].start_index);
            int eval = p.eval;
            beta = fmin(beta, eval);
            if (min_eval > eval)
            {
                min_eval = eval;
                bestMove = valid_moves[i];
            }
            if (beta <= alpha)
                break;
        }
        printf("Actual Best Move for black: %d || From %d to %d\n", min_eval, bestMove.start_index, bestMove.end_index);
        return (t_payload){min_eval, bestMove};
    }
}


t_bb copy_bitboard(t_bb *bitboards)
{
    t_bb new_bitboards;
    new_bitboards.black_pawns = bitboards->black_pawns;
    new_bitboards.black_knights = bitboards->black_knights;
    new_bitboards.black_bishops = bitboards->black_bishops;
    new_bitboards.black_rooks = bitboards->black_rooks;
    new_bitboards.black_queens = bitboards->black_queens;
    new_bitboards.black_king = bitboards->black_king;
    new_bitboards.black_pieces = bitboards->black_pieces;
    new_bitboards.black_attacks = bitboards->black_attacks;
    new_bitboards.white_pawns = bitboards->white_pawns;
    new_bitboards.white_knights = bitboards->white_knights;
    new_bitboards.white_bishops = bitboards->white_bishops;
    new_bitboards.white_rooks = bitboards->white_rooks;
    new_bitboards.white_queens = bitboards->white_queens;
    new_bitboards.white_king = bitboards->white_king;
    new_bitboards.white_pieces = bitboards->white_pieces;
    new_bitboards.white_attacks = bitboards->white_attacks;
    return (new_bitboards);
}


void process_AI(t_game game) {
    printf("Evaluating... %d\n", evaluate_board(*game.bitboards));
    t_move *valide_moves = malloc(sizeof(t_move) * 1000);
    generate_valid_moves(game, *(game.bitboards), game.white_to_play, valide_moves);



    t_payload p = minimax(game, *game.bitboards ,2, INT_MIN, INT_MAX, game.white_to_play);
    t_move best_move = p.move;


    //printf("Eval: %d\n", p.eval);

    
    int status = get_status_by_index(best_move.start_index, game.bitboards);
    make_move_bitboards(game.bitboards, status, best_move.start_index, best_move.end_index);

    free(valide_moves);
    update_bitboards(game.bitboards);
    update_gui(game.bitboards);
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




