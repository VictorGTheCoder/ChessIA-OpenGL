#include "../../includes/header.h"

//   northwest    north   northeast
//   NW             N          NE
//           >>9    >>8    >>7
//               \  |  /
//   W       >>1 <-  0 -> <<1    E
//               /  |  L
//           <<7   <<8    <<9
//   SW             S          SE
//   southwest    south   southeast

uint64_t get_diagonal_attacks(uint64_t position, uint64_t occupied_squares) {
    uint64_t attacks = 0;
    uint64_t sides = 0xFF818181818181FF;
    for (uint64_t pos = 1; pos != 0; pos <<= 1) {
        if ((pos & position) == 0) continue;
        for (int dir = 0; dir < 4; dir++) {
            uint64_t attack_bit = pos;
            while (1) {
                if (dir == 0) // NE
                {
                    if ((pos & 0x80808080808080FFULL)) //Check if pos is at right or top border
                        break;
                    attack_bit = (attack_bit >> 7);
                }
                else if (dir == 1) // NW
                {
                    if ((pos & 0x01010101010101FFULL)) //Check if pos is at left or top border
                        break;
                    attack_bit = (attack_bit >> 9);
                }
                else if(dir == 2) // SE
                {
                    if (pos & 0xFF80808080808080ULL) //Check if pos is at right or bottom border
                        break;
                    attack_bit = (attack_bit << 9);
                }
                else if (dir == 3) // SW
                {
                    if ((pos & 0xFF01010101010101ULL)) //Check if pos is at left or bottom border
                        break;
                    attack_bit = (attack_bit << 7);
                }
                if ((attack_bit) == 0) break;
                attacks |= attack_bit;
                if (attack_bit & occupied_squares) break;
                if (attack_bit & sides) break;
            }
        }
    }
    return attacks;
}



uint64_t get_horizontal_vertical_attacks(uint64_t position, uint64_t occupied_squares) {
    uint64_t attacks = 0;
    //uint64_t sides = 0xFF818181818181FF;
    for (uint64_t pos = 1; pos != 0; pos <<= 1) {
        if ((pos & position) == 0) continue;
        
        for (int dir = 0; dir < 4; dir++) {
            uint64_t attack_bit = pos;
            while (1) {
                if (dir == 0) // E
                {
                    if ((pos & 0x0101010101010101ULL))
                        break;
                    attack_bit = (attack_bit >> 1);
                    if (attack_bit & 0x8080808080808080ULL)
                        break;
                }
                else if (dir == 1) // W
                {
                    if ((pos & 0x8080808080808080ULL))
                        break;
                    attack_bit = (attack_bit << 1);
                    if (attack_bit & 0x0101010101010101ULL)
                        break;
                }
                else if (dir == 2) // N
                {
                    if ((pos & 0x00000000000000FFULL))
                        break;
                    attack_bit = (attack_bit >> 8);
                    if (attack_bit & 0xFF00000000000000ULL)
                        break;
                }
                else if (dir == 3) // S
                {
                    if ((pos & 0xFF00000000000000ULL))
                        break;
                    attack_bit = (attack_bit << 8);
                    if (attack_bit & 0x00000000000000FFULL) 
                        break;   
                }
                
                if ((attack_bit) == 0) break;
                attacks |= attack_bit;
                if (attack_bit & occupied_squares) break;
            }
        }
    }
    return attacks;
}



uint64_t generate_piece_attacks(int color, int piece_type, uint64_t position, t_bb *bb) {
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
            attacks |= get_diagonal_attacks(position, bb->white_pieces | bb->black_pieces);
            break;

        case ROOK:
            attacks |= get_horizontal_vertical_attacks(position, bb->white_pieces | bb->black_pieces);
            break;
        case QUEEN:
            attacks |= get_diagonal_attacks(position, bb->white_pieces | bb->black_pieces);
            attacks |= get_horizontal_vertical_attacks(position, bb->white_pieces | bb->black_pieces);
            break;
        case KING:
            // North
            attacks |= (position & 0xFFFFFFFFFFFFFF00ULL) >> 8;
            
            // North-East
            attacks |= (position & 0x7F7F7F7F7F7F7F00ULL) >> 7;
            
            // East
            attacks |= (position & 0x7F7F7F7F7F7F7F7FULL) << 1;
            
            // South-East
            attacks |= (position & 0x007F7F7F7F7F7F7FULL) << 9;
            
            // South
            attacks |= (position & 0x00FFFFFFFFFFFFFFULL) << 8;
            
            // South-West
            attacks |= (position & 0x00FEFEFEFEFEFEFEULL) << 7;
            
            // West
            attacks |= (position & 0xFEFEFEFEFEFEFEFEULL) >> 1;
            
            // North-West
            attacks |= (position & 0xFEFEFEFEFEFEFE00ULL) >> 9;
            break;

    }

    return attacks;
}