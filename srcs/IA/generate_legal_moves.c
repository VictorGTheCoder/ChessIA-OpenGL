#include "../../includes/header.h"

int is_legal_rook_move(t_bb *bb, int start_square, int end_square);
int is_legal_pawn_move(t_bb *bb, int start_square, int end_square);
int is_legal_king_move(t_bb *bb, int start_square, int end_square);
int is_legal_bishop_move(t_bb *bb, int start_square, int end_square);
int is_legal_knight_move(t_bb *bb, int start_square, int end_square);
int is_legal_queen_move(t_bb *bb, int start_square, int end_square);

static void add_move(t_move *move_list, int *move_count, int start_square, int end_square)
{
    if (move_list == NULL || move_count == NULL) {
        // Handle the error as appropriate for your system.
        fprintf(stderr, "Invalid pointer passed to add_move\n");
        return;
    }

    // Add the move to the list and increment the move count.
    move_list[*move_count].start_index = start_square;
    move_list[*move_count].end_index = end_square;
}

// Function to generate all legal pawn moves using bitboards.
// t_move *generate_legal_pawn_moves(t_game *game, int *move_count) {
//     // Allocate memory for the maximum number of pawn moves possible.
//     t_move *legal_moves = malloc(28 * sizeof(t_move)); // 28 to account for promotions
//     *move_count = 0;

//     // Get the bitboard for the pawns of the current player.
//     Bitboard pawns = game->white_to_play ? game->bitboards->white_pawns : game->bitboards->black_pawns;
    
//     while (pawns) {
//         // Get the index of the least significant bit set (LSB)
//         int start_square = __builtin_ffsll(pawns) - 1;
//         int end_square;

//         // Calculate the direction of the pawn movement based on the current player.
//         int direction = game->white_to_play ? 8 : -8;
//         int starting_rank = game->white_to_play ? 1 : 6;
//         int double_step_rank = game->white_to_play ? 3 : 4;
//         int promotion_rank = game->white_to_play ? 7 : 0;

//         // Check for single step move.
//         end_square = start_square + direction;
//         if (is_legal_pawn_move(game, start_square, end_square)) {
//             add_move(legal_moves, move_count, start_square, end_square);
//         }

//         // Check for double step move from the starting rank.
//         if ((start_square / 8) == starting_rank) {
//             end_square = start_square + (2 * direction);
//             if (is_legal_pawn_move(game, start_square, end_square)) {
//                 add_move(legal_moves, move_count, start_square, end_square);
//             }
//         }
//         pawns &= pawns - 1;
//     }

//     // Resize the move list to the actual number of legal moves found.
//     legal_moves = realloc(legal_moves, (*move_count) * sizeof(t_move));

//     return legal_moves;
// }

void generate_legal_pawn_moves(t_bb *bitboards, int *move_count, t_move *valid_moves, int is_white)
{
    // Get the bitboard for the pawns of the current player.
    Bitboard pawns = is_white? bitboards->white_pawns : bitboards->black_pawns;
    while (pawns)
    {
        int start_square = __builtin_ctzll(pawns);
        int end_square;

        int direction = is_white ? -8 : 8;
        int starting_rank = is_white ? 6 : 1;
        // int double_step_rank = is_white ? 4 : 2;
        // int promotion_rank = is_white ? 0 : 7;

        end_square = start_square + direction;
		printf("Start square: %d, end_square %d\n", start_square, end_square);
        if (is_legal_pawn_move(bitboards, start_square, end_square))
		{
            add_move(valid_moves, move_count, start_square, end_square);
        }

        // Check for double step move from the starting rank.
        if ((start_square / 8) == starting_rank)
		{
            end_square = start_square + (2 * direction);
            if (is_legal_pawn_move(bitboards, start_square, end_square))
			{
                add_move(valid_moves, move_count, start_square, end_square);
            }
        }
        pawns &= pawns - 1;
    }
}

void generate_legal_rook_moves(t_bb *bitboards, int *move_count, t_move *valid_moves, int is_white) {
    // Get the bitboard for the rooks of the current player.
    Bitboard rooks = is_white ? bitboards->white_rooks : bitboards->black_rooks;
    Bitboard own_pieces = is_white ? bitboards->white_pieces : bitboards->black_pieces;
    Bitboard opponents = is_white ? bitboards->black_pieces : bitboards->white_pieces;

    while (rooks) {
        int start_square = __builtin_ctzll(rooks);

        // Directions a rook can move: up, down, left, right
        int directions[] = {-8, 8, -1, 1};
        for (int i = 0; i < 4; i++) {
            int current_square = start_square;
            while (1) {
                current_square += directions[i];

                // Break if off board (for left and right movement)
                if (current_square < 0 || current_square >= 64) break;
                // For left movement, break if we're now in the 8th file
                if (directions[i] == -1 && current_square % 8 == 7) break;
                // For right movement, break if we're now in the 1st file
                if (directions[i] == 1 && current_square % 8 == 0) break;

                Bitboard destination_bitboard = 1ULL << current_square;
                
                // Check if the destination square is occupied by own piece
                if (destination_bitboard & own_pieces) break;
                
                // If it's a legal move (either empty square or capturing an opponent's piece), add it.
                                t_current_ply ply;
                ply.piece_type = ROOK | WHITE; //WARNING
                ply.white_to_play = is_white;
                ply.move_start = start_square;
                ply.move_end = current_square;
                if (!(destination_bitboard & opponents) || (destination_bitboard & opponents)) {
                    if (is_move_legal(bitboards, start_square, current_square, ply, is_white))
                        add_move(valid_moves, move_count, start_square, current_square);
                }
                
                // If the destination square is occupied by an opponent's piece, stop checking further in this direction.
                if (destination_bitboard & opponents) break;
            }
        }
        // Clear the bit for the current rook so we can move on to the next.
        rooks &= rooks - 1;
    }
}

void generate_legal_bishop_moves(t_bb *bitboards, int *move_count, t_move *valid_moves, int is_white) {
    Bitboard bishops = is_white ? bitboards->white_bishops : bitboards->black_bishops;

    while (bishops) {
        int start_square = __builtin_ctzll(bishops);

        // Directions a bishop can move: diagonals
        int directions[] = {-9, -7, 7, 9};
        for (int i = 0; i < 4; i++) {
            int current_square = start_square;
            while (1) {
                current_square += directions[i];

                // Check if the move is off the board (for diagonal movement)
                if (current_square < 0 || current_square >= 64) break;
                // Diagonal movement should not wrap around the board
                if ((current_square % 8 == 0 && directions[i] == -9) || // Left edge for up-left movement
                    (current_square % 8 == 7 && directions[i] == -7) || // Right edge for up-right movement
                    (current_square % 8 == 0 && directions[i] == 7) ||  // Left edge for down-left movement
                    (current_square % 8 == 7 && directions[i] == 9)) {  // Right edge for down-right movement
                    break;
                }
                t_current_ply ply;
                ply.piece_type = BISHOP | WHITE; //WARNING
                ply.white_to_play = is_white;
                ply.move_start = start_square;
                ply.move_end = current_square;
                if (is_move_legal(bitboards, start_square, current_square, ply, is_white)) {
                    add_move(valid_moves, move_count, start_square, current_square);
                } else {
                    // If move is not legal, it may be because the path is blocked, so stop checking further
                    break;
                }
            }
        }

        // Clear the bit for the current bishop so we can move on to the next
        bishops &= bishops - 1;
    }
}




int generate_valid_moves(t_game *game, t_bb bitboards, int is_white, t_move *valid_moves)
{
    int moves_count = 0;
	(void) valid_moves;
    (void) game;
	//valid_moves = malloc(sizeof(t_move) * 400);
	// // generate_legal_pawn_moves(game, &moves_count, valid_moves, is_white);
	// generate_legal_rook_moves(bitboards, &moves_count, valid_moves, is_white);
	// // generate_legal_bishop_moves(game, &moves_count, valid_moves, is_white);
	
	// printf("Number of legal bishop moves: %d\n", moves_count);

    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            if (is_white && get_bit(bitboards.white_pieces, i) == 0)
                continue;
            if (!is_white && get_bit(bitboards.black_pieces, i) == 0)
                continue;
            t_current_ply ply;
            ply.piece_type = get_status_by_index(i, &bitboards);
            ply.white_to_play = is_white;
            ply.move_end = j;
            ply.move_start = i;
            ply.target_status = get_status_by_index(j, &bitboards);
            if (ply.piece_type == EMPTY)
                continue;
            if (is_move_legal(&bitboards, i, j, ply, is_white))
            {
                add_move(valid_moves, &moves_count, i, j);
                moves_count++;
            }
            // if (try_to_move(game, &bitboards, i, j, is_white))
            // {
            //     add_move(valid_moves, &moves_count, i, j);
            //     moves_count++;
            // }
        }
        
    }
    printf("Legal moves count [%d]", moves_count);
    if (game->white_to_play == 1)
        printf(" for White\n");
    else
        printf(" for Black\n");
    //exit(EXIT_FAILURE);
    return (moves_count);
}

