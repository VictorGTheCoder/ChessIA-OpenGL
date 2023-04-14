
#include "../../includes/header.h"

extern t_game game;

void remove_piece(t_case *square, t_gui *gui)
{

	square->square_img = 0;
	square->status = EMPTY;
}

void deselect_piece(t_gui *gui, t_case *square)
{
	gui->square_selected = NULL;
}

void move_piece(t_gui *gui, t_case *start_square, t_case *end_square)
{
	// Vörifier si c'est un coup en passant
	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == PAWN && end_square == game.en_passant_target) {
		end_square->status = EMPTY;
		game.en_passant_target = NULL;
	}
	end_square->status = start_square->status;
	start_square->status = EMPTY;
	end_square->square_img = start_square->square_img;
	remove_piece(start_square, gui);
	gui->square_selected = NULL;
	glutPostRedisplay();
}

int is_valid_pawn_move(t_gui *gui, t_case *start_square, t_case *end_square)
{
	int direction = (start_square->status & WHITE) ? -1 : 1;
	int diffX = abs((end_square->startX - start_square->startX) / 100);
	int diffY = (end_square->startY - start_square->startY) / 100;

	// avancer d'une case
	if (diffX == 0 && diffY == direction && end_square->status == EMPTY)
	{
		return 1;
	}

	// prise diagonale
	if (diffX == 1 && diffY == direction && (end_square->status & (BLACK | WHITE)) != (start_square->status & (BLACK | WHITE)) && end_square->status != EMPTY)
	{
		return 1;
	}

	// en passant
	if (end_square == game.en_passant_target)
	{
		t_case *square = is_white_piece(start_square) == 1 ? end_square->case_s : end_square->case_n;
		remove_piece(square, gui);
		return 1;
	}

	// Vérifier si le pion est à sa position initiale et si les deux cases devant sont vides
	if (diffX == 0 && diffY == 2 * direction && ((start_square->status & WHITE) ? start_square->startY == 6 * 100 : start_square->startY == 1 * 100))
	{
		// avancer de deux cases depuis la position initiale
		t_case *middle_square = (direction == 1) ? &gui->case_list[get_square_from_xy(start_square->startX, start_square->startY + 100)] : &gui->case_list[get_square_from_xy(start_square->startX, start_square->startY - 100)];
		if (middle_square->status == EMPTY && end_square->status == EMPTY)
		{
			return 1;
		}
	}

	return 0;
}

int is_valid_bishop_move(t_gui *gui, t_case *start_square, t_case *end_square)
{
    int nbs_start = get_square_from_xy(start_square->startX, start_square->startY);
    int nbs_end = get_square_from_xy(end_square->startX, end_square->startY);
    int i;

    int diffX = abs(end_square->startX - start_square->startX);
    int diffY = abs(end_square->startY - start_square->startY);

    // Vérifier si les déplacements sont diagonaux
    if (diffX != diffY)
        return 0;

    int step;
    if (end_square->startX > start_square->startX)
    {
        if (end_square->startY > start_square->startY)
            step = 9; // Diagonale bas-droite
        else
            step = 7; // Diagonale haut-droite
    }
    else
    {
        if (end_square->startY > start_square->startY)
            step = 7; // Diagonale bas-gauche
        else
            step = 9; // Diagonale haut-gauche
    }

    if (nbs_end > nbs_start)
    {
        i = nbs_start + step;
        while (i != nbs_end)
        {
            if (gui->case_list[i].status != EMPTY)
                return 0;

            i += step;
        }
        return 1;
    }
    else
    {
        i = nbs_start - step;
        while (i != nbs_end)
        {
            if (gui->case_list[i].status != EMPTY)
                return 0;

            i -= step;
        }
        return 1;
    }
}

int is_valid_rook_move(t_gui *gui, t_case *start_square, t_case *end_square)
{
    int diffX = abs(end_square->startX - start_square->startX);
    int diffY = abs(end_square->startY - start_square->startY);

    // Vérifier si les deux positions sont sur la même ligne ou colonne
    if (diffX != 0 && diffY != 0)
    {
        return 0;
    }

    int directionX = (diffX == 0) ? 0 : (end_square->startX > start_square->startX ? 100 : -100);
    int directionY = (diffY == 0) ? 0 : (end_square->startY > start_square->startY ? 100 : -100);

    int currentX = start_square->startX + directionX;
    int currentY = start_square->startY + directionY;

    while (currentX != end_square->startX || currentY != end_square->startY)
    {
        int index = get_square_from_xy(currentX, currentY);
        if (gui->case_list[index].status != EMPTY)
        {
            return 0;
        }
        currentX += directionX;
        currentY += directionY;
    }

    // Vérifier si la case d'arrivée est vide ou contient une pièce de l'adversaire
    if ((end_square->status & (BLACK | WHITE)) != (start_square->status & (BLACK | WHITE)) || end_square->status == EMPTY)
    {
		if (game.white_to_play == 1 && get_square_from_xy(start_square->startX, start_square->startY) == 63)
		{
			game.white_can_castle_king_side = 0;
		}
		if (game.white_to_play == 1 && get_square_from_xy(start_square->startX, start_square->startY) == 56)
		{
			game.white_can_castle_queen_side = 0;
		}
		if (game.white_to_play == 0 && get_square_from_xy(start_square->startX, start_square->startY) == 0)
		{
			game.black_can_castle_queen_side = 0;
		}
		if (game.white_to_play == 0 && get_square_from_xy(start_square->startX, start_square->startY) == 7)
		{
			game.black_can_castle_king_side = 0;
		}
        return 1;
    }

    return 0;
}

int is_valid_knight_move(t_gui *gui, t_case *start_square, t_case *end_square)
{
    int diffX = abs(end_square->startX - start_square->startX);
    int diffY = abs(end_square->startY - start_square->startY);

    // Vérifier si le mouvement est en forme de L (2 cases dans une direction et 1 case dans l'autre direction)
    if ((diffX == 200 && diffY == 100) || (diffX == 100 && diffY == 200))
    {
        // Vürifier si la case d'arrivée est vide ou contient une pièce de l adversaire
        if ((end_square->status & (BLACK | WHITE)) != (start_square->status & (BLACK | WHITE)) || end_square->status == EMPTY)
        {
			printf("Valide knight move\n");
            return 1;
        }
    }
    return 0;
}

int is_valid_king_move(t_gui *gui, t_case *start_square, t_case *end_square)
{
    int diffX = abs(end_square->startX - start_square->startX) / 100;
    int diffY = abs(end_square->startY - start_square->startY) / 100;
    int is_white = is_white_piece(start_square);

    // Vérifier si le mouvement est valide (une case dans n'importe quelle direction)
    if ((diffX <= 1 && diffY <= 1) && ((end_square->status & (BLACK | WHITE)) != (start_square->status & (BLACK | WHITE)) || end_square->status == EMPTY))
    {
        return 1;
    }
	else if (diffY == 0 && diffX == 2 &&  is_king_in_check(gui, is_white) == 0)
	{
		printf("King not in check\n");
	 	if (game.white_to_play && (game.white_can_castle_king_side || game.white_can_castle_queen_side))
			return 2;
		if (game.white_to_play == 0 && (game.black_can_castle_king_side || game.black_can_castle_queen_side))
			return 2;
		return (0); //Cannot castle
	}
	else
	{
		return 0;
	} 
}

int move_is_conform(t_gui *gui, t_case *start_square, t_case *end_square)
{
	//printf("Start square x: %d, y:%d, END square x: %d, y:%d\n", start_square->startX, start_square->startY, end_square->startX, end_square->endY);
	if ((start_square->status & (BLACK | WHITE)) == (end_square->status & (BLACK | WHITE)))
		return 0;
	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == PAWN)
	{
		return (is_valid_pawn_move(gui, start_square, end_square));
	}
	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == BISHOP)
	{
		return (is_valid_bishop_move(gui, start_square, end_square));
	}
	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == ROOK)
	{
		return (is_valid_rook_move(gui, start_square, end_square));
	}
	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == QUEEN)
	{
		return (is_valid_bishop_move(gui, start_square, end_square) || is_valid_rook_move(gui, start_square, end_square));
	}
	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == KNIGHT)
	{
    	return (is_valid_knight_move(gui, start_square, end_square));
	}
	if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == KING)
	{
		return (is_valid_king_move(gui, start_square, end_square)); 
	}
	return 0;
}

int move_is_valid(t_gui *gui, t_case *start_square, t_case *end_square)
{
	int	r;
	t_gui *temp_gui = clone_t_gui(gui);
	if (!temp_gui)
		printf("Memory Allocation error. You may have no space left on your computer\n");
	t_case *temp_start_square = &temp_gui->case_list[get_square_from_xy(start_square->startX, start_square->startY)];
	t_case *temp_end_square = &temp_gui->case_list[get_square_from_xy(end_square->startX, end_square->startY)];	
	if ((r = move_is_conform(temp_gui, temp_start_square, temp_end_square)))
	{
		//print_board_in_term(temp_gui);
		printf("R %d\n", r);
		if (r == 2) //king try castling
		{
			printf("CAST<li\n");
			/*if (temp_start_square->startX - temp_end_square->startX > 0)
			{
				printf("isok1\n");
				move_piece(temp_gui, temp_start_square, temp_start_square->case_w);
				//print_board_in_term(temp_gui);
				if (is_king_in_check(temp_gui, game.white_to_play) == 1)
					return (0);
				move_piece(temp_gui, temp_start_square->case_w, temp_start_square);
			}
			else
			{
				printf("isok2\n");
				move_piece(temp_gui, temp_start_square, temp_start_square->case_e);
				if (is_king_in_check(temp_gui, game.white_to_play) == 1)
					return (0);
				move_piece(temp_gui, temp_start_square->case_e, temp_start_square);
			}*/
		}
		move_piece(temp_gui, temp_start_square, temp_end_square);

		if (is_king_in_check(temp_gui, game.white_to_play) == 1)
		{	
			print_board_in_term(temp_gui);
			free(temp_gui);
			return (0);
		}
		/*move_is_conform(gui, start_square, end_square); //On effectue le rock
		printf("Move is valid\n");
		if (game.white_to_play)
		{
			game.white_can_castle_queen_side = 0;
			game.white_can_castle_king_side = 0;
		}*/
		free(temp_gui->case_list);
		free(temp_gui);
		return (r);
	}
	return (r);
}

int	check_castling(t_gui *gui, t_case *start_square, t_case *end_square)
{

	int diffX = abs(end_square->startX - start_square->startX) / 100;
    int diffY = abs(end_square->startY - start_square->startY) / 100;
    int is_white = is_white_piece(start_square);

	if (is_white)
        {
			printf("Statusssss %d, %d\n", gui->case_list[61].status,gui->case_list[62].status);
            if (end_square->startX > start_square->startX && game.white_can_castle_king_side &&
                gui->case_list[61].status == EMPTY && gui->case_list[62].status == EMPTY)
            {
				move_piece(gui, &gui->case_list[63], &gui->case_list[61]); // Move rook
				game.white_can_castle_queen_side = 0;
				game.white_can_castle_king_side = 0;
                return 2; // Roque côté roi pour les blancs
            }
            else if (end_square->startX < start_square->startX && game.white_can_castle_queen_side &&
                     gui->case_list[59].status == EMPTY && gui->case_list[58].status == EMPTY && gui->case_list[57].status == EMPTY)
            {
				move_piece(gui, &gui->case_list[56], &gui->case_list[59]);// Move rook
				game.white_can_castle_queen_side = 0;
				game.white_can_castle_king_side = 0;
                return 3; // Roque côté dame pour les blancs
            }
			else return -1;
        }
        else
        {
            if (end_square->startX > start_square->startX && game.black_can_castle_king_side &&
                gui->case_list[5].status == EMPTY && gui->case_list[6].status == EMPTY)
            {
				move_piece(gui, &gui->case_list[7], &gui->case_list[5]); // Move rook
				game.black_can_castle_queen_side = 0;
				game.black_can_castle_king_side = 0;
                return 4; // Roque côté roi pour les noirs
            }
            else if (end_square->startX < start_square->startX && game.black_can_castle_queen_side &&
                    gui->case_list[1].status == EMPTY && gui->case_list[2].status == EMPTY && gui->case_list[3].status == EMPTY)
            {
				move_piece(gui, &gui->case_list[0], &gui->case_list[3]); // Move rook
				game.black_can_castle_queen_side = 0;
				game.black_can_castle_king_side = 0;
                return 5; // Roque côté dame pour les noirs
            }
			else return -2;
        }
}

int try_to_move(t_gui *gui, t_case *start_square, t_case *end_square)
{
    //printf("Start Square %d, End square %d, King is in check %d Can castling %d, %d, %d, %d\n", get_square_from_xy(start_square->startX, start_square->startY), get_square_from_xy(end_square->startX, end_square->startY), is_king_in_check(gui, game.white_to_play), game.white_can_castle_king_side, game.white_can_castle_queen_side, game.black_can_castle_king_side, game.black_can_castle_queen_side);
    int	r =  move_is_valid(gui, start_square, end_square);
    if (r) // move is valid
    {	
		// Mise à jour de la cible en passant avant d'appeler la fonction move
        if ((start_square->status & (PAWN | BISHOP | ROOK | QUEEN | KNIGHT | KING)) == PAWN && abs((end_square->startY - start_square->startY) / 100) == 2)
        {
            game.en_passant_target = (start_square->startY > end_square->startY) ? end_square->case_n : end_square->case_s;
        }
        else
        {
            game.en_passant_target = NULL;
        }
		if (r == 2)
			printf("Check rock %d\n", check_castling(gui, start_square, end_square));
		// Appliquer le mouvement sur la structure principale 'gui'
        move_piece(gui, start_square, end_square);
		if (game.white_to_play == 0)
            game.white_to_play = 1;
        else
            game.white_to_play = 0;
        game.is_piece_selected = 0;
        return 1; // Indique que le mouvement est valide et a été effectué
    }
    return r;
}
