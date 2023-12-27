#include "../../includes/header.h"


clock_t total;

int get_piece_value(int status)
{
	switch (status)
	{
		case WHITE | PAWN:
			return 1;
		case WHITE | BISHOP:
			return 3;
        case WHITE | KNIGHT:
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
        case BLACK | KNIGHT:
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
    update_bitboards(&bitboards);
	int total_score = 0;
	for (int i = 0; i < 64; i++)
	{
		if (get_bit(bitboards.black_pieces | bitboards.white_pieces, i) == 1)
        {
            //printf("Piece %d: %d\n", i, get_piece_value(get_status_by_index(i, &bitboards)));
            total_score += get_piece_value(get_status_by_index(i, &bitboards));
        }
	}
   // printf("Total score: %d\n", total_score);
	return (total_score);
}
	int EvaluatedPositions = 0;


t_payload QuiescenceSearch(t_game game, t_bb bitboards, int alpha, int beta, int maximizing_player)
{
    t_move bestMove;
    t_move valid_moves[1000];
    int moveCount;

	bestMove.end_index =0;
	moveCount = generate_valid_moves(game, bitboards, maximizing_player, valid_moves, 1);
	if (moveCount == 0)
	{
		return (t_payload){evaluate_board(bitboards), bestMove};
	}
	if (maximizing_player)
    {
        for (int i = 0; i < moveCount; i++)
        {
            int status = get_status_by_index(valid_moves[i].start_index, &bitboards);
            make_move_bitboards(&bitboards, status, valid_moves[i].start_index, valid_moves[i].end_index);
            t_payload p = QuiescenceSearch(game, bitboards, alpha, beta, !maximizing_player);
            make_move_bitboards(&bitboards, status, valid_moves[i].end_index, valid_moves[i].start_index);
            int eval = p.eval;

            if (eval > alpha)
                bestMove = valid_moves[i];

            alpha = (int)fmax(alpha, eval);
            if (beta <= alpha)
                break;
        }
        return (t_payload){alpha, bestMove};
    }
    else
    {
        for (int i = 0; i < moveCount; i++)
        {
            int status = get_status_by_index(valid_moves[i].start_index, &bitboards);
            make_move_bitboards(&bitboards, status, valid_moves[i].start_index, valid_moves[i].end_index);
            t_payload p = QuiescenceSearch(game, bitboards, alpha, beta, !maximizing_player);
            make_move_bitboards(&bitboards, status, valid_moves[i].end_index, valid_moves[i].start_index);
            int eval = p.eval;

            if (eval < beta)
                bestMove = valid_moves[i];

            beta = (int) fmin(beta, eval);
            if (beta <= alpha)
                break;
        }
        return (t_payload){beta, bestMove};
    }

}


t_payload Search (t_game game, t_bb bitboards, int depth, int alpha, int beta, int maximizing_player)
{
    t_move bestMove;
    t_move valid_moves[1000];
    int moveCount;


    bestMove.start_index = 0;
    bestMove.end_index = 0;

	EvaluatedPositions++;

    if (depth == 0)
        return QuiescenceSearch(game, bitboards, alpha, beta, maximizing_player);
        //return (t_payload){evaluate_board(bitboards), bestMove};
    

    clock_t start = clock();
    moveCount = generate_valid_moves(game, bitboards, maximizing_player, valid_moves, 0);
    clock_t end = clock();
    total += (end - start);

    if (maximizing_player)
    {
        for (int i = 0; i < moveCount; i++)
        {
            int status = get_status_by_index(valid_moves[i].start_index, &bitboards);
            make_move_bitboards(&bitboards, status, valid_moves[i].start_index, valid_moves[i].end_index);
            t_payload p = Search(game, bitboards, depth - 1, alpha, beta, !maximizing_player);
            make_move_bitboards(&bitboards, status, valid_moves[i].end_index, valid_moves[i].start_index);
            int eval = p.eval;

            if (eval > alpha)
                bestMove = valid_moves[i];

            alpha = (int)fmax(alpha, eval);
            if (beta <= alpha)
                break;
        }
        return (t_payload){alpha, bestMove};
    }
    else
    {
        for (int i = 0; i < moveCount; i++)
        {
            int status = get_status_by_index(valid_moves[i].start_index, &bitboards);
            make_move_bitboards(&bitboards, status, valid_moves[i].start_index, valid_moves[i].end_index);
            t_payload p = Search(game, bitboards, depth - 1, alpha, beta, !maximizing_player);
            make_move_bitboards(&bitboards, status, valid_moves[i].end_index, valid_moves[i].start_index);
            int eval = p.eval;

            if (eval < beta)
                bestMove = valid_moves[i];

            beta = (int) fmin(beta, eval);
            if (beta <= alpha)
                break;
        }
        return (t_payload){beta, bestMove};
    }
}

void process_AI(t_game game) {

    printf("Evaluating... %d\n", evaluate_board(*game.bitboards));
	

    total = 0;
    delta2 = 0;
	EvaluatedPositions = 0;
    
    clock_t start = clock();
    t_payload p = Search(game, *game.bitboards , 2, INT_MIN, INT_MAX, game.white_to_play);
	clock_t end = clock();
    printf("Time taken: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("Time taken: %fs\n", (double)(total) / CLOCKS_PER_SEC);
    printf("Time taken: %fs\n", (double)(delta2) / CLOCKS_PER_SEC);
    
    
    printf("Evaluated %d positions\n", EvaluatedPositions);
    
    t_move best_move = p.move;
    int eval = p.eval;

    


    printf("Eval: %d\n", eval);

    
    int status = get_status_by_index(best_move.start_index, game.bitboards);
    make_move_bitboards(game.bitboards, status, best_move.start_index, best_move.end_index);

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




