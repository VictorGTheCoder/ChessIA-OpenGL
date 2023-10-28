#include "../../includes/header.h"

int is_legal_rook_move(t_game *game, int start_square, int end_square) {
    int start_rank = start_square / 8;
    int start_file = start_square % 8;
    
    int end_rank = end_square / 8;
    int end_file = end_square % 8;
    
    // If not in the same rank and not in the same file, it's an illegal move
    if (start_rank != end_rank && start_file != end_file) {
        return 0;
    }

    int step;
    uint64_t mask;
    
    if (start_rank == end_rank) { // Moving horizontally
        step = (start_file < end_file) ? 1 : -1;
        for (int file = start_file + step; file != end_file; file += step) {
            mask = 1ULL << (start_rank * 8 + file);
            if (game->bitboards->white_pieces & mask || game->bitboards->black_pieces & mask) {
                return 0; // Path is blocked
            }
        }
    } else { // Moving vertically
        step = (start_rank < end_rank) ? 8 : -8;
        for (int square = start_square + step; square != end_square; square += step) {
            mask = 1ULL << square;
            if (game->bitboards->white_pieces & mask || game->bitboards->black_pieces & mask) {
                return 0; // Path is blocked
            }
        }
    }

    // Check if the destination square is empty or has an opponent's piece
    mask = 1ULL << end_square;
    if ((game->bitboards->white_pieces & mask && game->white_to_play) ||
        (game->bitboards->black_pieces & mask && !game->white_to_play)) {
        return 0;
    }

    return 1; // Legal move
}




int is_move_legal(int start_square, int end_case)
{
	int result = 0;
    switch (gui->case_list[start_square].status & COLOR_MASK)
	{
        case ROOK:
            result = is_legal_rook_move(game, start_square, end_case);    
            break;
        case PAWN:
			return 1;
            //result = is_legal_pawn_move(game, start_square, end_case);
        /*case KNIGHT:
            result = is_legal_knight_move(game, start_square, end_case);
            break;
        case BISHOP:
            result = is_legal_bishop_move(game, start_square, end_case);
            break;
        case QUEEN:
            result = is_legal_queen_move(game, start_square, end_case);
            break;
        case KING:
            result = is_legal_king_move(game, start_square, end_case);
            break;
            break;*/
        default:
            break;
	}
	return (result);
}



