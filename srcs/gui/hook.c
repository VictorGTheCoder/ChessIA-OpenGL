#include "../../includes/header.h"

void key_hook(unsigned char key, int x, int y)
{
	if (key == 'q')
	{
		exit(0);
	}
	printf("key %c, x %d, y %d\n", key, x, y);
}

void mouse_hook(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
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
			
			//Si la selected piece et la target ont la même couleur
			if (game->white_to_play == is_white_piece(square))
			{
				deselect_piece(gui, gui->square_selected);
				case_selected(gui, square);
			}
			else
			{
				if (gui->square_selected != NULL)
				{

					if ((try_to_move(game->bitboards, get_square_from_xy(gui->square_selected->startX, gui->square_selected->startY), square_n, game->white_to_play) == -1))
					{
						case_selected(gui, square);
					}
				}
			}
		}
		else
		{
			if (square->status != EMPTY)
			{
				case_selected(gui, square);
				game->is_piece_selected = 1;
			}
			else
			{
				game->is_piece_selected = 0;
				deselect_piece(gui, gui->square_selected);
			}
		}
    }
}

