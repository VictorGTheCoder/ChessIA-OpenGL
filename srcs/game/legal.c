#include "../../includes/header.h"

int is_legal_pawn_move(t_bb *bitboards, int start_square, int end_square, int is_white) {
    int start_rank = 7 - start_square / 8;
    int start_file = start_square % 8;
    int end_rank = 7 - end_square / 8;
    int end_file = end_square % 8;
    

    printf("Start (%d, %d) end (%d, %d)\n", start_rank, start_file, end_rank, end_file);
    uint64_t mask = 1ULL << end_square;
    
    if (is_white) {
        if (end_file == start_file) {
            if ((start_rank == 1 && end_rank == start_rank + 2 && !(bitboards->black_pieces & mask)) || // Double step from starting position
                (end_rank == start_rank + 1 && !(bitboards->black_pieces & mask))) { // Single step
                return 1;
            }
        } else if (abs(end_file - start_file) == 1 && end_rank == start_rank + 1 && (bitboards->black_pieces & mask)) { // Capturing
            return 1;
        }
    } else {
        if (end_file == start_file) {
            if ((start_rank == 6 && end_rank == start_rank - 2 && !(bitboards->white_pieces & mask)) || // Double step from starting position
                (end_rank == start_rank - 1 && !(bitboards->white_pieces & mask))) { // Single step
                return 1;
            }
        } else if (abs(end_file - start_file) == 1 && end_rank == start_rank - 1 && (bitboards->white_pieces & mask)) { // Capturing
            return 1;
        }
    }
    printf("Illegal move\n");
    return 0; // Illegal move
}

int is_legal_rook_move(t_bb *bitboards, int start_square, int end_square) {
    int start_rank = start_square / 8;
    int start_file = start_square % 8;
    int end_rank = end_square / 8;
    int end_file = end_square % 8;
    
    // If not in the same rank and not in the same file, it's an illegal move
    if (start_rank != end_rank && start_file != end_file) {
        printf("Not the same rank or file\n");
        return 0;
    }

    int step;
    uint64_t mask;
    
    if (start_rank == end_rank) { // Moving horizontally
        step = (start_file < end_file) ? 1 : -1;
        for (int file = start_file + step; file != end_file; file += step) {
            mask = 1ULL << (start_rank * 8 + file);
            if (bitboards->white_pieces & mask || bitboards->black_pieces & mask) {
                printf("Path is blocked 1\n");
                return 0; // Path is blocked
            }
        }
    } else { // Moving vertically
        step = (start_rank < end_rank) ? 8 : -8;
        for (int square = start_square + step; square != end_square; square += step) {
            mask = 1ULL << square;
            if (bitboards->white_pieces & mask || bitboards->black_pieces & mask) {
                printf("Path is blocked 2\n");
                return 0; // Path is blocked
            }
        }
    }

    // // Check if the destination square is empty or has an opponents piece
    // mask = 1ULL << end_square;
    // if ((game->bitboards->white_pieces & mask && is_white) ||
    //     (game->bitboards->black_pieces & mask && !is_white)) {
    //     printf("Destination Error\n");
    //     return 0;
    // }

    return 1;
}

int is_legal_bishop_move(t_bb *bitboards, int start_square, int end_square) {
    int diff_rank = abs((end_square / 8) - (start_square / 8));
    int diff_file = abs((end_square % 8) - (start_square % 8));
    
    if (diff_rank != diff_file) {
        return 0; // Not moving diagonally
    }
    
    int rank_step = (end_square / 8 > start_square / 8) ? 8 : -8;
    int file_step = (end_square % 8 > start_square % 8) ? 1 : -1;
    int step = rank_step + file_step;
    printf("Start %d end %d\n", start_square, end_square);
    
    for (int square = start_square + step; square != end_square; square += step) {
        uint64_t mask = 1ULL << square;
        if (bitboards->white_pieces & mask || bitboards->black_pieces & mask) {
            return 0; // Path is blocked
        }
    }
    
    // Check if the destination square is empty or has an opponent's piece
    // uint64_t mask = 1ULL << end_square;
    // if ((game->bitboards->white_pieces & mask && is_white) ||
    //     (game->bitboards->black_pieces & mask && !is_white)) {
    //     return 0;
    // }
    
    return 1;
}

int is_legal_knight_move(t_bb *bb, int start_square, int end_square) {
    (void) bb;
    int diff_rank = abs((end_square / 8) - (start_square / 8));
    int diff_file = abs((end_square % 8) - (start_square % 8));
    
    // Check L shape move
    if (!((diff_rank == 2 && diff_file == 1) || (diff_rank == 1 && diff_file == 2))) {
        return 0;
    }
    
    // Check if the destination square is empty or has an opponent's piece
    // uint64_t mask = 1ULL << end_square;
    // if ((game->bitboards->white_pieces & mask && is_white) ||
    //     (game->bitboards->black_pieces & mask && !is_white)) {
    //     return 0;
    // }
    
    return 1;
}

int is_legal_queen_move(t_bb *bb, int start_square, int end_square) {
    return is_legal_rook_move(bb, start_square, end_square) ||
           is_legal_bishop_move(bb, start_square, end_square);
}

int is_legal_king_move(t_bb *bb, int start_square, int end_square) {
    (void) bb;
    int diff_rank = abs((end_square / 8) - (start_square / 8));
    int diff_file = abs((end_square % 8) - (start_square % 8));
    
    if (diff_rank > 1 || diff_file > 1) {
        return 0; // Not a single square move
    }
    
    // Check if the destination square is empty or has an opponent's piece
    // uint64_t mask = 1ULL << end_square;
    // if ((game->bitboards->white_pieces & mask && is_white) ||
    //     (game->bitboards->black_pieces & mask && !is_white)) {
    //     return 0;
    // }
    
    return 1; // Legal move
}

int is_king_in_check(t_bb bb, int is_white_to_play) {
    uint64_t king_position;
    
    if (is_white_to_play == 1)
    {
        printf("White is playing\n");
    }
    else
        printf("Black is playing\n");
    // Determine the position of the king
    king_position = (is_white_to_play == 1) ? bb.white_king : bb.black_king;
    

    // printf("King position\n");
    // print_bitboard(king_position);
    // Check if the king is attacked
    if ((is_white_to_play == 1 && (bb.black_attacks & king_position)) || 
        (is_white_to_play == 0 && (bb.white_attacks & king_position))) {
        return 1; // King is in check
    }
    
    return 0; // King is not in check
}

int is_king_in_check_after_move(int white_to_play, t_bb bb, int piece_type, int start_square, int end_square, t_current_ply ply) {
    // Simulate the move
    // printf("Start_square %d\n", start_square);
    // printf("End_square %d\n", end_square);
    (void) bb;
    make_move_bitboards(&bb, piece_type, start_square, end_square);

    // Bitboard opBoard;

    // if (game->white_to_play == 1)
    //     opBoard = bb.black_pieces;
    // else
    //     opBoard = bb.white_pieces;
    
    update_bitboards(&bb);
    int eaten_piece = check_if_a_piece_is_eaten(ply, bb);
    if (eaten_piece != EMPTY)
    {
        printf("<--------PIECE EATEN ------->\n");
        delete_piece_from_bitboard(end_square, getBoard(&bb, eaten_piece));
        update_bitboards(&bb);

    }
    else
    {
        printf("<--------NO PIECE EATEN ------->\n");

    }

    // printf("Oponent Bitboard after move\n");
    // print_bitboard(opBoard);
    // //print_bitboard(bb->white_attacks);

    // printf("Bitboard after move\n");
    // print_bitboard(bb.black_pieces);
    // printf("--------------------\n");
    // print_bitboard(bb.white_pieces);

    // printf("White attacks\n");
    // print_bitboard(bb.white_attacks);
    // printf("Black attacks\n");
    // print_bitboard(bb.black_attacks);
    int in_check = is_king_in_check(bb, white_to_play);
    return in_check;
}

int is_move_legal(t_bb *bitboards, int start_square, int end_case, t_current_ply ply, int is_white)
{
	int result = 0;
    if (is_king_in_check_after_move(is_white, *bitboards, ply.piece_type, start_square, end_case, ply))
    {
        printf("[ILLEGAL MOVE] King is check\n");
       return (0); // Move puts king in check
    } 
    
    switch (ply.piece_type & COLOR_MASK)
	{
        case PAWN:
			result = is_legal_pawn_move(bitboards, start_square, end_case, is_white);
            break;
        case ROOK:
            result = is_legal_rook_move(bitboards, start_square, end_case);    
            break;
        case KNIGHT:
            result = is_legal_knight_move(bitboards, start_square, end_case);
            break;
        case BISHOP:
            result = is_legal_bishop_move(bitboards, start_square, end_case);
            break;
        case QUEEN:
            result = is_legal_queen_move(bitboards, start_square, end_case);
            break;
        case KING:
            result = is_legal_king_move(bitboards, start_square, end_case);
            break;
	}
	return (result);
}
