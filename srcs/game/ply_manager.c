#include "../../includes/header.h"

int	check_for_end_game(t_bb *b, t_game *game)
{

	t_move *valid_moves = malloc(400 * sizeof(t_move));
	int nb_valid_moves = generate_valid_moves(game, *b, game->white_to_play, valid_moves);
	free(valid_moves);
	if (nb_valid_moves == 0)
	{
		printf("=====================\n");
		printf("=                   =\n");
		printf("=                   =\n");
		if (is_king_in_check(*b, game->white_to_play))
		{
			if (game->white_to_play)
				printf("= Checkmate, white wins =\n\n");
			else
				printf("= Checkmate, black wins =\n\n");
		}
		else
		{
			printf("=  Stalemate, draw  =\n");
		}
		printf("=                   =\n");
		printf("=                   =\n");
		printf("=====================\n");

		return (0);
	}
	
	return (1);
}


void manage_click(int x, int y)
{
	if (gui->square_selected)
		printf("Selected square (%d, %d)\n", gui->square_selected->startX, gui->square_selected->startY);
	else
		printf("No square selected\n");
	int square_n = get_square_from_xy(x, y);
	t_case *square = &gui->case_list[square_n];
	//printf("isSelected %d, isWhiteToPlay %d, square id %d, pieceIsWhite %d, square status %d, image_status %d\n", game->is_piece_selected, game->white_to_play, square_n, is_white_piece(square), square->status, square->square_img);
	if (game->is_piece_selected == 1)
	{
		//printf("Square selected %d, x:%d, y%d\n", game->is_piece_selected, gui->square_selected->startX, gui->square_selected->startY);
		
		//Si selected piece et la target ont la même couleur
		if (game->white_to_play == is_white_piece(square))
		{
			deselect_piece(gui->square_selected);
			case_selected(square);
		}
		else
		{
			if (gui->square_selected != NULL)
			{	
				int rt = try_to_move(game, game->bitboards, get_square_from_xy(gui->square_selected->startX, gui->square_selected->startY), square_n, game->white_to_play);
				if ((rt == 0))
				{
					
				}
				else if (rt == 1)
				{
					update_bitboards(game->bitboards);
					update_gui(game->bitboards);
					switch_ply(game);
					if (check_for_end_game(game->bitboards, game) == 0)
					{
						exit(EXIT_SUCCESS);
					}
				}
			}
		}
	}
	else
	{
		if (square->status != EMPTY)
		{
			case_selected(square);
			game->is_piece_selected = 1;
		}
		else
		{
			game->is_piece_selected = 0;
			deselect_piece(gui->square_selected);
		}
	}
}