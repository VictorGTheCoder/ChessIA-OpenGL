#include "../../includes/header.h"

int is_legal_pawn_move(t_game *game, int start_square, int end_square) {
    int start_rank = 7 - start_square / 8;
    int start_file = start_square % 8;
    int end_rank = 7 - end_square / 8;
    int end_file = end_square % 8;
    

    printf("Start (%d, %d) end (%d, %d)\n", start_rank, start_file, end_rank, end_file);
    uint64_t mask = 1ULL << end_square;
    
    if (game->white_to_play) {
        if (end_file == start_file) {
            if ((start_rank == 1 && end_rank == start_rank + 2 && !(game->bitboards->black_pieces & mask)) || // Double step from starting position
                (end_rank == start_rank + 1 && !(game->bitboards->black_pieces & mask))) { // Single step
                return 1;
            }
        } else if (abs(end_file - start_file) == 1 && end_rank == start_rank + 1 && (game->bitboards->black_pieces & mask)) { // Capturing
            return 1;
        }
    } else {
        if (end_file == start_file) {
            if ((start_rank == 6 && end_rank == start_rank - 2 && !(game->bitboards->white_pieces & mask)) || // Double step from starting position
                (end_rank == start_rank - 1 && !(game->bitboards->white_pieces & mask))) { // Single step
                return 1;
            }
        } else if (abs(end_file - start_file) == 1 && end_rank == start_rank - 1 && (game->bitboards->white_pieces & mask)) { // Capturing
            return 1;
        }
    }
    printf("Illegal move\n");
    return 0; // Illegal move
}

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

    // Check if the destination square is empty or has an opponents piece
    mask = 1ULL << end_square;
    if ((game->bitboards->white_pieces & mask && game->white_to_play) ||
        (game->bitboards->black_pieces & mask && !game->white_to_play)) {
        return 0;
    }

    return 1;
}

int is_legal_bishop_move(t_game *game, int start_square, int end_square) {
    int diff_rank = abs((end_square / 8) - (start_square / 8));
    int diff_file = abs((end_square % 8) - (start_square % 8));
    
    if (diff_rank != diff_file) {
        return 0; // Not moving diagonally
    }
    
    int rank_step = (end_square / 8 > start_square / 8) ? 8 : -8;
    int file_step = (end_square % 8 > start_square % 8) ? 1 : -1;
    int step = rank_step + file_step;
    
    for (int square = start_square + step; square != end_square; square += step) {
        uint64_t mask = 1ULL << square;
        if (game->bitboards->white_pieces & mask || game->bitboards->black_pieces & mask) {
            return 0; // Path is blocked
        }
    }
    
    // Check if the destination square is empty or has an opponent's piece
    uint64_t mask = 1ULL << end_square;
    if ((game->bitboards->white_pieces & mask && game->white_to_play) ||
        (game->bitboards->black_pieces & mask && !game->white_to_play)) {
        return 0;
    }
    
    return 1;
}

int is_legal_knight_move(t_game *game, int start_square, int end_square) {
    int diff_rank = abs((end_square / 8) - (start_square / 8));
    int diff_file = abs((end_square % 8) - (start_square % 8));
    
    // Check L shape move
    if (!((diff_rank == 2 && diff_file == 1) || (diff_rank == 1 && diff_file == 2))) {
        return 0;
    }
    
    // Check if the destination square is empty or has an opponent's piece
    uint64_t mask = 1ULL << end_square;
    if ((game->bitboards->white_pieces & mask && game->white_to_play) ||
        (game->bitboards->black_pieces & mask && !game->white_to_play)) {
        return 0;
    }
    
    return 1;
}

int is_legal_queen_move(t_game *game, int start_square, int end_square) {
    return is_legal_rook_move(game, start_square, end_square) ||
           is_legal_bishop_move(game, start_square, end_square);
}

int is_legal_king_move(t_game *game, int start_square, int end_square) {
    int diff_rank = abs((end_square / 8) - (start_square / 8));
    int diff_file = abs((end_square % 8) - (start_square % 8));
    
    if (diff_rank > 1 || diff_file > 1) {
        return 0; // Not a single square move
    }
    
    // Check if the destination square is empty or has an opponent's piece
    uint64_t mask = 1ULL << end_square;
    if ((game->bitboards->white_pieces & mask && game->white_to_play) ||
        (game->bitboards->black_pieces & mask && !game->white_to_play)) {
        return 0;
    }
    
    return 1; // Legal move
}



int is_king_in_check(t_game *game, int color) {
    uint64_t king_position;
    
    // Determine the position of the king
    king_position = (color == WHITE) ? game->bitboards->white_king : game->bitboards->black_king;
    
    // Check if the king is attacked
    if ((color == WHITE && (game->bitboards->black_attacks & king_position)) || 
        (color == BLACK && (game->bitboards->white_attacks & king_position))) {
        return 1; // King is in check
    }
    
    return 0; // King is not in check
}

int is_king_in_check_after_move(t_bb *bb, int piece_type, int start_square, int end_square) {
    // Simulate the move
    printf("Start_square %d\n", start_square);
    printf("End_square %d\n", end_square);

    update_bitboards(bb, piece_type, start_square, end_square);
    //printf("TESTTEST\n\n\n");
    int in_check = is_king_in_check(game, game->white_to_play);
    update_bitboards(bb, piece_type, end_square, start_square);
    return 0;
}


int is_move_legal(int start_square, int end_case)
{
	int result = 0;
    if (is_king_in_check_after_move((game->bitboards), gui->case_list[start_square].status, start_square, end_case))
        return (0); // Move puts king in check
    
    switch (gui->case_list[start_square].status & COLOR_MASK)
	{
        case PAWN:
			result = is_legal_pawn_move(game, start_square, end_case);
            break;
        case ROOK:
            result = is_legal_rook_move(game, start_square, end_case);    
            break;
        case KNIGHT:
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
	}
	return (result);
}