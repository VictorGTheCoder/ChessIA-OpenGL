#include "../../includes/header.h"
/* BOARD INDEX
0  1  2  3  4  5  6  7
8  9  10 11 12 13 14 15
16 17 18 19 20 21 22 23
24 25 26 27 28 29 30 31
32 33 34 35 36 37 38 39
40 41 42 43 44 45 46 47
48 49 50 51 52 53 54 55
56 57 58 59 60 61 62 63
*/

void update_bitboards(t_bb *bitboards, int piece_type, int start_square, int end_square)
{    
    Bitboard *bb;
    switch (piece_type)
    {
        case PAWN | WHITE:
            bb = &bitboards->white_pawns;
            break;
        case PAWN | BLACK:
            bb = &bitboards->black_pawns;
            break;
        case ROOK | WHITE:
            bb = &bitboards->white_rooks;
            break;
        case ROOK | BLACK:
            bb = &bitboards->black_rooks;
            break;
        case BISHOP | WHITE:
            bb = &bitboards->white_bishops;
            break;
        case BISHOP | BLACK:
            bb = &bitboards->black_bishops;
            break;
        case KING | WHITE:
            bb = &bitboards->white_king;
            break;
        case KING | BLACK:
            bb = &bitboards->black_king;
            break;
        case QUEEN | WHITE:
            bb = &bitboards->white_queens;
            break;
        case QUEEN | BLACK:
            bb = &bitboards->black_queens;
            break;
    }


    uint64_t start_mask = 1ULL << (start_square);
    uint64_t end_mask = 1ULL << (end_square);

    *bb &= ~start_mask; // Clear the bit at the start square
    *bb |= end_mask;    // Set the bit at the end square

    bitboards->white_pieces =  bitboards->white_pawns |  bitboards->white_knights | 
                                bitboards->white_bishops |  bitboards->white_rooks | 
                                bitboards->white_queens |  bitboards->white_king;

    bitboards->black_pieces =  bitboards->black_pawns |  bitboards->black_knights | 
                                bitboards->black_bishops |  bitboards->black_rooks | 
                                bitboards->black_queens |  bitboards->black_king;
}


void print_bitboard(uint64_t bitboard) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%d ", (bitboard & mask) ? 1 : 0);
        }
        printf("\n");
    }
}

void print_combined_bitboard(t_bb *bitboards) {
    uint64_t combined = bitboards->white_pawns   | bitboards->white_knights | bitboards->white_bishops |
                        bitboards->white_rooks   | bitboards->white_queens  | bitboards->white_king    |
                        bitboards->black_pawns   | bitboards->black_knights | bitboards->black_bishops |
                        bitboards->black_rooks   | bitboards->black_queens  | bitboards->black_king;
    
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square_index = rank * 8 + file;
            if ((1ULL << square_index) & combined) {
                printf("1 ");
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
    printf("\n");
}



void initialize_bitboards() {
    game->bitboards = malloc(sizeof(t_bb));
    // Pawns
    game->bitboards->black_pawns = 0x000000000000FF00;
    game->bitboards->white_pawns = 0x00FF000000000000;

    // Knights
    game->bitboards->black_knights = 0x0000000000000042;
    game->bitboards->white_knights = 0x4200000000000000;

    // Bishops
    game->bitboards->black_bishops = 0x0000000000000024;
    game->bitboards->white_bishops = 0x2400000000000000;

    // Rooks
    game->bitboards->black_rooks = 0x0000000000000081;
    game->bitboards->white_rooks = 0x8100000000000000;

    // Queens
    game->bitboards->black_queens = 0x0000000000000010;
    game->bitboards->white_queens = 0x1000000000000000;

    // Kings
    game->bitboards->black_king = 0x0000000000000008;
    game->bitboards->white_king = 0x0800000000000000;

    // All white pieces
    game->bitboards->white_pieces = game->bitboards->white_pawns | 
                                   game->bitboards->white_knights | 
                                   game->bitboards->white_bishops | 
                                   game->bitboards->white_rooks | 
                                   game->bitboards->white_queens | 
                                   game->bitboards->white_king;

    // All black pieces
    game->bitboards->black_pieces = game->bitboards->black_pawns | 
                                   game->bitboards->black_knights | 
                                   game->bitboards->black_bishops | 
                                   game->bitboards->black_rooks | 
                                   game->bitboards->black_queens | 
                                   game->bitboards->black_king;
}



int get_bit(Bitboard board, int index) {
    return (board >> index) & 1;
}

void set_bit(Bitboard *board, int index, int value) {
    if (value)
        *board |= (1ULL << index);
    else
        *board &= ~(1ULL << index);
}


void move_piece_bb(Bitboard *start_board, Bitboard *end_board, int start_index, int end_index)
{
    set_bit(end_board, end_index, 1);
    set_bit(start_board, start_index, 0);
}



t_game *clone_t_game(t_game *game) {
    t_game *new_game = malloc(sizeof(t_game));
    if (!new_game) {
        // Handle memory allocation error
        return NULL;
    }

    return new_game;
}

void free_t_game(t_game *game)
{
    free(game);
}


