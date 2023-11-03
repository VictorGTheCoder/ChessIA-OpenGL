
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
}

int try_to_move(int start_square, int end_square) {
    t_current_ply c_ply;

    c_ply.move_start = 0;
    c_ply.move_end = 0;
    t_case *start_case = &(gui->case_list[start_square]);
    t_case *end_case = &(gui->case_list[end_square]);

    set_bit(&c_ply.move_start, start_square, 1);
    printf("Start Move\n");
    print_bitboard(c_ply.move_start);
    printf("END Move\n");

    set_bit(&c_ply.move_end, end_square, 1);
    print_bitboard(c_ply.move_end);
    // Check if move is legal accorldy to the type of piece
    c_ply.piece_type = start_case->status;
    if (is_move_legal(start_square, end_square, c_ply))
    {
        
        if (end_case->status & 0b11000 == WHITE)
        {
            if (!game->white_to_play)
            {
                set_bit(getBoard(end_case->status) ,get_square_from_xy(end_case->startX, end_case->startY), 0); 
            }
        }
        else if (end_case->status & 0b11000 == BLACK)
        {
            if (game->white_to_play)
            {
                set_bit(getBoard(end_case->status) ,get_square_from_xy(end_case->startX, end_case->startY), 0); 
            }
        }
        update_bitboards(game->bitboards, start_case->status, start_square, end_square);
        //print_bitboard(game->bitboards->white_knights);
        //print_combined_bitboard(game->bitboards);

        
        move_piece(gui, start_case, end_case);

        switch_ply();
        return (1);
    }
    printf("Illegal move\n");

    //print_combined_bitboard(game->bitboards);
    /*if (start_case->status != EMPTY)
    {
        //move_piece(gui, start_case, end_case);
        if (game->white_to_play == 1)
            game->white_to_play = 0;
        else
            game->white_to_play = 1;
        return (1);
    }*/

    
    //return (0);
    // Function assumes that the bitboard representation and game state are well synchronized
    // Ensure you have functions to update game state based on bitboard changes and vice versa

    // Call your move validation function here (Assuming it’s updated to use bitboards)
    /*int move_valid = move_is_valid(game, start_square, end_square);

    if (move_valid) {
        // Update en passant target
        game->en_passant_target = update_en_passant(game, start_square, end_square);

        // Update castling rights
        update_castling_rights(game, start_square, end_square);

        // Perform the move on the bitboards
        perform_move_on_bitboards(game->bitboards, start_square, end_square);

        // Toggle the side to move
        game->white_to_play = !game->white_to_play;

        // If it’s the AI’s turn, call the AI function
        if (!game->white_to_play) {
            process_AI(game);
            game->white_to_play = !game->white_to_play;
        }

        // Reset piece selection flag
        game->is_piece_selected = 0;

        // Return 1 indicating that the move was performed
        return 1;
    }*/

    // Return the result of the move validation (0 or error code)
    return 0;
}

