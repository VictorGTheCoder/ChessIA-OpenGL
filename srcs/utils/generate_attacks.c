#include "../../includes/header.h"

uint64_t get_diagonal_attacks(uint64_t position, uint64_t occupied_squares) {
    uint64_t attacks = 0;
    int directions[] = {-7, -9, 9, 7}; // NE, NW, SE, SW
    
    for (int dir = 0; dir < 4; dir++) {
        uint64_t attack_bit = position;
        while (42) {
			printf("test\n");
            if (dir < 2) // Moving upwards (NE, NW)
                attack_bit = (attack_bit << directions[dir]);
            else // Moving downwards (SE, SW)
                attack_bit = (attack_bit >> -directions[dir]);
            
            // Border and occupancy checks
            if ((attack_bit & 0xFEFEFEFEFEFEFEFE) == 0 || // Check file H borders
                (attack_bit & 0x7F7F7F7F7F7F7F7F) == 0 || // Check file A borders
                (attack_bit & occupied_squares)) {
                break;
            }
            
            attacks |= attack_bit;
            
            // Additional check to prevent sliding over the edge of the board
            if ((attack_bit & 0x7F7F7F7F7F7F7F7F) == 0 && (directions[dir] == 7 || directions[dir] == -9) || // Check for left border
                (attack_bit & 0xFEFEFEFEFEFEFEFE) == 0 && (directions[dir] == 9 || directions[dir] == -7)) { // Check for right border
                break;
            }
        }
    }
    
    return attacks;
}


uint64_t get_horizontal_vertical_attacks(uint64_t position, uint64_t occupied_squares) {
    uint64_t attacks = 0;
    int directions[] = {-8, -1, 1, 8}; // N, W, E, S
    
    for (int dir = 0; dir < 4; dir++) {
        uint64_t attack_bit = position;
        while (42) {
            if (directions[dir] == -1 || directions[dir] == 1) // Horizontal (W, E)
                attack_bit = (directions[dir] == -1) ? (attack_bit << 1) : (attack_bit >> 1);
            else // Vertical (N, S)
                attack_bit = (directions[dir] == -8) ? (attack_bit << 8) : (attack_bit >> 8);
            
            // Check if move is outside the board or hits an occupied square
            if ((attack_bit & 0x8080808080808080) == 0 || // Check for file H
                (attack_bit & 0x0101010101010101) == 0 || // Check for file A
                (attack_bit & occupied_squares)) {
                break;
            }
            
            attacks |= attack_bit;
            
            // Additional checks for horizontal moves not to cross the file borders
            if ((attack_bit & 0x8080808080808080) && directions[dir] == 1 || // Right move check
                (attack_bit & 0x0101010101010101) && directions[dir] == -1) { // Left move check
                break;
            }
        }
    }
    
    return attacks;
}


uint64_t generate_piece_attacks(int color, int piece_type, uint64_t position) {
    uint64_t attacks = 0;
    uint64_t temp_positions;

    switch (piece_type) {
        case PAWN:
            if (color == BLACK) {
                // Move diagonally left
                temp_positions = (position & 0x7F7F7F7F7F7F7F7FULL) << 9;
                attacks |= temp_positions;

                // Move diagonally right
                temp_positions = (position & 0xFEFEFEFEFEFEFEFEULL) << 7;
                attacks |= temp_positions;
            } else {
                // Move diagonally left
                temp_positions = (position & 0xFEFEFEFEFEFEFEFEULL) >> 9;
                attacks |= temp_positions;

                // Move diagonally right
                temp_positions = (position & 0x7F7F7F7F7F7F7F7FULL) >> 7;
                attacks |= temp_positions;
            }
            break;

        case KNIGHT:
            // Left 2, Up 1
            temp_positions = (position & 0xFCFCFCFCFCFCFCFCULL) << 6;
            attacks |= temp_positions;

            // Right 2, Up 1
            temp_positions = (position & 0x3F3F3F3F3F3F3F3FULL) << 10;
            attacks |= temp_positions;

            // Left 2, Down 1
            temp_positions = (position & 0xFCFCFCFCFCFCFCFCULL) >> 10;
            attacks |= temp_positions;

            // Right 2, Down 1
            temp_positions = (position & 0x3F3F3F3F3F3F3F3FULL) >> 6;
            attacks |= temp_positions;

            // Left 1, Up 2
            temp_positions = (position & 0xFEFEFEFEFEFEFEFEULL) << 15;
            attacks |= temp_positions;

            // Right 1, Up 2
            temp_positions = (position & 0x7F7F7F7F7F7F7F7FULL) << 17;
            attacks |= temp_positions;

            // Left 1, Down 2
            temp_positions = (position & 0xFEFEFEFEFEFEFEFEULL) >> 17;
            attacks |= temp_positions;

            // Right 1, Down 2
            temp_positions = (position & 0x7F7F7F7F7F7F7F7FULL) >> 15;
            attacks |= temp_positions;
            break;
		case BISHOP:
            // Calculate diagonal attacks from the bishop's position
            // Note: Implement get_diagonal_attacks function that handles obstructions
            attacks = get_diagonal_attacks(position, game->bitboards->white_pieces | game->bitboards->black_pieces);
            break;

        case ROOK:
            // Calculate horizontal and vertical attacks from the rook's position
            // Note: Implement get_horizontal_vertical_attacks function that handles obstructions
            attacks = get_horizontal_vertical_attacks(position, game->bitboards->white_pieces | game->bitboards->black_pieces);
            break;
    }

    return attacks;
}