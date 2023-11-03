
#include "../../includes/header.h"

void remove_piece(t_case *square, t_gui *gui)
{
	square->square_img = 0;
	square->status = EMPTY;
}

void deselect_piece(t_gui *gui, t_case *square)
{
	gui->square_selected = NULL;
    game->is_piece_selected = 0;
}

void move_piece(t_gui *gui, t_case *start_square, t_case *end_square)
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
	remove_piece(start_square, gui);
	gui->square_selected = NULL;
	glutPostRedisplay();
}

void switch_ply()
{
    if (game->white_to_play == 1)
        game->white_to_play = 0;
    else
        game->white_to_play = 1;

    printf("\n\nswitch ply\n\n");
}

int check_if_a_piece_is_eaten(t_current_ply c_ply)
{
    if (c_ply.piece_type == EMPTY)
        return (0);
    if (game->white_to_play)
    {
        int index = __builtin_ctzll(c_ply.move_end);
        if (c_ply.move_end && game->bitboards->black_pawns == 1)
            return (1);
    }
    else
    {
        int index = __builtin_ctzll(c_ply.move_end);
        if (c_ply.move_end && game->bitboards->white_pawns == 1)
            return (1);
    }

    // if (end_case->status == EMPTY)
    //     return ;
    // int is_white = is_white_piece(end_case) == 1;
    // if (is_white == 1)
    // {
    //     if (!game->white_to_play)
    //     {
    //         set_bit(getBoard(end_case->status) ,get_square_from_xy(end_case->startX, end_case->startY), 0); 
    //         printf("White piece %d is dead at square %d\n", end_case->status, get_square_from_xy(end_case->startX, end_case->startY));

    //     }
    // }
    // else if (is_white == 0)
    // {
    //     if (game->white_to_play)
    //     {

            
    //         printf("Black piece %d is dead at square %d\n", end_case->status, get_square_from_xy(end_case->startX, end_case->startY));
    //     }
    // }
    // else if (is_white == -1)
    // {
    //     printf("Error: Piece is not white or black\n");
    // }
}


int try_to_move(int start_square, int end_square) {
    t_current_ply c_ply;

    c_ply.move_start = 0;
    c_ply.move_end = 0;
    t_case *start_case = &(gui->case_list[start_square]);
    t_case *end_case = &(gui->case_list[end_square]);

    // set_bit(&c_ply.move_start, start_square, 1);
    // printf("Start Move\n");
    // print_bitboard(c_ply.move_start);
    // printf("END Move\n");

    set_bit(&c_ply.move_end, end_square, 1);
    print_bitboard(c_ply.move_end);
    c_ply.piece_type = start_case->status;
    // Check if move is legal accorldy to the type of piece
    if (is_move_legal(start_square, end_square, c_ply))
    {
        if (check_if_a_piece_is_eaten(c_ply))
        {
            printf("<--------PIECE EATEN ------->\n");
            delete_piece_from_bitboard(end_square, getBoard(c_ply.piece_type));
        }
        update_bitboards(game->bitboards, start_case->status, start_square, end_square);
        printf("<======BLACK======>\n");
        print_bitboard(game->bitboards->black_pieces);
        printf("<======WHITE======>\n");
        print_bitboard(game->bitboards->white_pieces);
        //print_bitboard(game->bitboards->white_knights);
        //print_combined_bitboard(game->bitboards);

        
        move_piece(gui, start_case, end_case);

        switch_ply();
        return (1);
    }
    printf("Illegal move\n");

    return 0;
}

