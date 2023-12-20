#include "../../includes/header.h"

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
				if ((try_to_move(game, game->bitboards, get_square_from_xy(gui->square_selected->startX, gui->square_selected->startY), square_n, game->white_to_play) == 0))
				{
					case_selected(square);
				}
				else
				{

					update_gui(game->bitboards);

					printf("BOARD STATUS\n");
					for (int i = 0; i < 64; i++)
					{

						printf("%d ", gui->case_list[i].status);
						
						if (i % 8 == 7)
							printf("\n");
					}
					
					switch_ply(game);


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