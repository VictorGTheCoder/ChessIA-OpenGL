
#include "../../includes/header.h"

void remove_piece(t_case *square)
{
	square->square_img = 0;
	square->status = EMPTY;
}

void deselect_piece(t_case *square)
{
    (void) square;
	gui->square_selected = NULL;
    game->is_piece_selected = 0;
}

void move_piece(t_case *start_square, t_case *end_square)
{
	// Vörifier si c'est un coup en passant
	if (get_square_from_xy(end_square->startX, end_square->startY) == 0)
		game->black_can_castle_queen_side = 0;
	else if (get_square_from_xy(end_square->startX, end_square->startY) == 7)
		game->black_can_castle_king_side = 0;
	else if (get_square_from_xy(end_square->startX, end_square->startY) == 56)
		game->white_can_castle_queen_side = 0;
	else if (get_square_from_xy(end_square->startX, end_square->startY) == 63)
		game->white_can_castle_king_side = 0;

	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == PAWN && end_square == game->en_passant_target) {
		end_square->status = EMPTY;
		game->en_passant_target = NULL;
	}
	end_square->status = start_square->status;
	start_square->status = EMPTY;
	end_square->square_img = start_square->square_img;
	remove_piece(start_square);
	gui->square_selected = NULL;
	glutPostRedisplay();
}

void switch_ply(t_game *game)
{
    if (game->white_to_play == 1)
        game->white_to_play = 0;
    else
        game->white_to_play = 1;

    print_combined_bitboard(game->bitboards);

    printf("\n\n<---------- NEXT ROUND -------->\n");
    if (game->white_to_play == 1)
        printf("White to play\n");
    else
        printf("Black to play\n");
    


}

int check_if_a_piece_is_eaten(t_current_ply c_ply, t_bb bb)
{
    if (c_ply.target_status == EMPTY)
        return (0);
    // printf("Move end\n");
    // print_bitboard(c_ply.move_end);
    if (c_ply.white_to_play)
    {
        if (c_ply.move_end & bb.black_bishops)
            return BISHOP | BLACK;
        if (c_ply.move_end & bb.black_rooks)
            return ROOK | BLACK;
        if (c_ply.move_end & bb.black_queens)
            return QUEEN | BLACK;
        if (c_ply.move_end & bb.black_knights)
            return KNIGHT | BLACK;
        if (c_ply.move_end & bb.black_pawns)
            return PAWN | BLACK;
        if (c_ply.move_end & bb.black_king)
            return KING | BLACK;
    }
    else
    {
        if (c_ply.move_end & bb.white_bishops)
            return BISHOP | WHITE;
        if (c_ply.move_end & bb.white_rooks)
            return ROOK | WHITE;
        if (c_ply.move_end & bb.white_queens)
            return QUEEN | WHITE;
        if (c_ply.move_end & bb.white_knights)
            return KNIGHT | WHITE;
        if (c_ply.move_end & bb.white_pawns)
            return PAWN | WHITE;
        if (c_ply.move_end & bb.white_king)
            return KING | WHITE;
    }
    return (EMPTY);
}


int try_to_move(t_game *game, t_bb *bitboards, int start_square, int end_square, int is_white)
{
    t_current_ply c_ply;

    c_ply.move_start = 0;
    c_ply.move_end = 0;
    c_ply.white_to_play = game->white_to_play;

    // set_bit(&c_ply.move_start, start_square, 1);
    // printf("Start Move\n");
    // print_bitboard(c_ply.move_start);
    // printf("END Move\n");

    // set_bit(&c_ply.move_end, end_square, 1);
    // print_bitboard(c_ply.move_end);
    c_ply.piece_type = get_status_by_index(start_square, bitboards);
    c_ply.target_status = get_status_by_index(end_square, bitboards);
    set_bit(&c_ply.move_start, start_square, 1);
    set_bit(&c_ply.move_end, end_square, 1);
    // Check if move is legal accorldy to the type of piece


    if (c_ply.piece_type == EMPTY)
    {
        return 0;
    }
    if (is_move_legal(bitboards, start_square, end_square, c_ply, is_white))
    {
        if (c_ply.target_status != EMPTY)
        {
            delete_piece_from_bitboard(end_square, getBoard(game->bitboards, c_ply.target_status));
        }
        make_move_bitboards(game->bitboards, c_ply.piece_type, start_square, end_square);
        // if (check_if_a_piece_is_eaten(c_ply))
        // {
        //     printf("<--------PIECE EATEN MAIN ------->\n");
            

        // }
        // update_bitboards(game->bitboards);
        // printf("<======BLACK PIECES======>\n");
        // print_bitboard(game->bitboards->black_pieces);
        // printf("<-- Attacks  board -->\n");
        // print_bitboard(game->bitboards->black_attacks);
        // printf("<======WHITE PIECES======>\n");
        // print_bitboard(game->bitboards->white_pieces);
        // printf("<-- Attacks  board -->\n");
        // print_bitboard(game->bitboards->white_attacks);

        //update_gui(gui, game->bitboards);
        


        //process_AI(*game);
        return (1);
    }
    printf("Illegal move\n");

    return 0;
}

